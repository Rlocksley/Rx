#include "LoadSkeletonModel.hpp"
#include "LoadDataHelper.hpp"
#include "SkeletonMeshArray.hpp"
#include "TextureMaterialBuffer.hpp"
#include "IndirectBuffer.hpp"
#include "TextureArray.hpp"
#include "Material.hpp"
#include "VkSkeletonMeshArray.hpp"
#include "VkTextureMaterialBuffer.hpp"
#include "VkIndirectBuffer.hpp"
#include "VkTransformBuffer.hpp"
#include "VkTextureArray.hpp"
#include "VkSkeletonModelDescriptorSet.hpp"
#include "VkShadowSkeletonModelDescriptorSet.hpp"
#include "GameWorldBase.hpp"
#include "LoadTextureModel.hpp"
#include "NodeTransform.hpp"
#include "Node.hpp"

#include "AnimationBone.hpp"
#include "AnimationClip.hpp"
#include "AnimationMap.hpp"
#include "Skeleton.hpp"
#include "VkSkeleton.hpp"
#include "VkKeyFrameArrayBuffer.hpp"
#include "KeyFrameBuffer.hpp"
#include "VkSkeletonModelCompDescriptorSet.hpp"
#include "SkeletonBuffer.hpp"
#include "LocalTransform.hpp"
#include "SkeletonInstance.hpp"
#include "NodeChildren.hpp"
#include "Transform.hpp"
#include "SkeletonEntityArray.hpp"
#include "SkeletonIndex.hpp"

namespace Rx {

namespace Asset{
    void createParentIndex(std::vector<Component::Node>& nodes, uint32_t& nodeIndex, uint32_t parentIndex) {
        nodes[nodeIndex].parentIndex = parentIndex;
        uint32_t currentNodeIndex = nodeIndex;
        for (uint32_t i = 0; i < nodes[currentNodeIndex].numberChildren; i++) {
            createParentIndex(nodes, ++nodeIndex, currentNodeIndex);
        }
    }


     void createAnimationBones(flecs::world& world, const std::vector<Component::Node>& nodes, const std::vector<Component::NodeName>& nodeNames, uint32_t& nodeIndex, uint32_t animationClipIndex, std::vector<Component::AnimationBone>& animationBones, std::vector<std::vector<std::vector<Component::PositionKey>>>& positionKeys, std::vector<std::vector<std::vector<Component::RotationKey>>>& rotationKeys, std::vector<std::vector<std::vector<Component::ScalingKey>>>& scalingKeys) {

            if(nodes[nodeIndex].isBone){
                animationBones.push_back(std::move(Rx::Component::AnimationBone(
                        nodeNames[nodeIndex].name,
                        std::move(positionKeys[animationClipIndex][nodes[nodeIndex].boneIndex]),
                        std::move(rotationKeys[animationClipIndex][nodes[nodeIndex].boneIndex]),
                        std::move(scalingKeys[animationClipIndex][nodes[nodeIndex].boneIndex])
                    )));
            }
            
			uint32_t currentNodeIndex = nodeIndex;
            for(int j = 0; j < nodes[currentNodeIndex].numberChildren; j++){
                createAnimationBones(world, nodes, nodeNames, ++nodeIndex, animationClipIndex, animationBones, positionKeys, rotationKeys, scalingKeys);
            }
        }


    void createKeyFrames(const std::vector<Component::AnimationBone>& animationBones, std::vector<std::vector<Component::KeyFrame>>& keyFrames, std::vector<float>& keyFrameTimes, float keyFrameDiff) {
        
        std::vector<float> allKeyFrameTimes;
        for(uint32_t i = 0; i < animationBones.size(); i++){
            auto keyFrameTimesBone = animationBones[i].getKeyFrameTimes();
            allKeyFrameTimes.insert(allKeyFrameTimes.end(), keyFrameTimesBone.begin(), keyFrameTimesBone.end());
        }

        std::sort(allKeyFrameTimes.begin(), allKeyFrameTimes.end());

        keyFrameTimes.push_back(allKeyFrameTimes[0]);
        // Merge key frames that are too close together
        // This is to avoid having too many key frames that are very close to each other

        for (size_t i = 1; i < allKeyFrameTimes.size(); ++i) {
            if (allKeyFrameTimes[i] - keyFrameTimes.back() >= keyFrameDiff) {
                keyFrameTimes.push_back(allKeyFrameTimes[i]);
            }
        }

        // Create KeyFrames for each key frame time
        keyFrames.reserve(keyFrameTimes.size());
        for(auto& keyFrameTime : keyFrameTimes){
            std::vector<Component::KeyFrame> keyFramesSkeleton;
            keyFramesSkeleton.reserve(animationBones.size());
            for(auto& animationBone : animationBones){
                auto [position, rotation, scaling] = animationBone.getLocalTransform(keyFrameTime);
                keyFramesSkeleton.push_back(Component::KeyFrame{
                    position,
                    rotation,
                    scaling
                });
            }
            keyFrames.push_back(std::move(keyFramesSkeleton));
        }
    }

    flecs::entity /*assetEntity*/ loadSkeletonModel(const std::string& filePath, flecs::world& world, const std::string& assetName, const uint32_t maxNumberInstances) {

            // Read the entire file into a vector<char>
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file) {
            RX_LOGE("Asset", "convertToTextureModel", ("Failed to open skeleton model file: " + filePath).c_str());
        }
        file.seekg(0, std::ios::end);
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<char> data(static_cast<size_t>(fileSize));
        if (!file.read(data.data(), fileSize)) {
            RX_LOGE("Asset", "convertToTextureModel", ("Failed to open skeleton model file: " + filePath).c_str());
        }

        uint64_t offset = 0;
        MainSkeletonModelHeader header;

        loadData(data, header.numberMeshes, offset);
        loadData(data, header.numberNodes, offset);
        loadData(data, header.numberBones, offset);
        loadData(data, header.numberAnimations, offset);
        loadData(data, header.numberTextures, offset);

        auto asset = world.entity(assetName.c_str());
        Rx::Component::SkeletonMeshArray meshArray;
        Rx::Component::TextureMaterialBuffer textureMaterialBuffer;
        Rx::Component::IndirectBuffer indirectBuffer;
        Rx::Component::TextureArray textureArray;

        for (uint32_t i = 0; i < header.numberMeshes; i++) {
            Rx::Component::TextureMaterial material;
            loadData(data, material.textureIndex, offset);
            loadData(data, material.albedo, offset);
            loadData(data, material.metalRough, offset);
            loadData(data, material.emissive, offset);
            textureMaterialBuffer.addMaterial(material);
        }

        for (uint32_t i = 0; i < header.numberMeshes; i++) {
            SkeletonMeshHeader meshHeader;
            loadString(data, meshHeader.name, 64, offset);
            loadData(data, meshHeader.numVertices, offset);
            loadData(data, meshHeader.numIndices, offset);

            std::vector<Rx::Vertex::Skeleton> vertices(meshHeader.numVertices);
            for(uint32_t j = 0; j < meshHeader.numVertices; j++){
                loadData(data, vertices[j].position, offset);
                loadData(data, vertices[j].normal, offset);
                loadData(data, vertices[j].texCoord, offset);
                loadData(data, vertices[j].bones, offset);
                loadData(data, vertices[j].weights, offset);
                loadData(data, vertices[j].nodeIndex, offset);
            }
            
            std::vector<uint32_t> indices(meshHeader.numIndices);
            memcpy(indices.data(), data.data() + offset, meshHeader.numIndices * sizeof(uint32_t));
            offset += meshHeader.numIndices * sizeof(uint32_t);

            meshArray.addMesh(meshHeader.name, vertices, indices);
            indirectBuffer.addCommand(meshArray.getCommand(meshHeader.name));
        }
        

        std::vector<TextureHeader> textureHeaders(header.numberTextures);
        for (uint32_t i = 0; i < header.numberTextures; i++) {
            loadString(data, textureHeaders[i].name, 64, offset);
            loadData(data, textureHeaders[i].size, offset);
            loadData(data, textureHeaders[i].width, offset);
            loadData(data, textureHeaders[i].height, offset);
        }

        std::vector<Rx::Core::TextureCPU> texturesCpu(header.numberTextures);
        for (uint32_t i = 0; i < header.numberTextures; i++) {
            auto& textureCpu = texturesCpu[i];
            textureCpu.width = textureHeaders[i].width;
            textureCpu.height = textureHeaders[i].height;
            textureCpu.pixels = malloc(textureHeaders[i].size);
            for(int j = 0; j < textureHeaders[i].size; j++){
                loadData(data, ((char*)textureCpu.pixels)[j], offset);
            }
            textureCpu.vkFormat = VK_FORMAT_R8G8B8A8_SRGB;
            textureArray.addTexture(textureHeaders[i].name, textureCpu);
        }

        std::vector<Component::Node> nodes(header.numberNodes);
        std::vector<Component::NodeName> nodeNames(header.numberNodes);

        for(uint32_t i = 0; i < nodes.size(); i++){
            loadData(data, nodes[i].isBone, offset);
            loadString(data, nodeNames[i].name, 128, offset);
            loadData(data, nodes[i].offset, offset);
            loadData(data, nodes[i].numberChildren, offset);
			loadData(data, nodes[i].nodeIndex, offset);
            loadData(data, nodes[i].boneIndex, offset);
        }

        uint32_t nodeIndex = 0; // Start from the first node
        createParentIndex(nodes, nodeIndex, -1); // Create parent indices for all nodes

        std::vector<AnimationHeader> animationHeaders(header.numberAnimations);
        std::vector<std::vector<AnimationBoneHeader>> animationBoneHeaders(header.numberAnimations);
        std::vector<std::vector<std::vector<Component::PositionKey>>> positionKeys(header.numberAnimations);
        std::vector<std::vector<std::vector<Component::RotationKey>>> rotationKeys(header.numberAnimations);
        std::vector<std::vector<std::vector<Component::ScalingKey>>> scalingKeys(header.numberAnimations);

        for(uint32_t i = 0; i < animationHeaders.size(); i++){
            loadString(data, &animationHeaders[i].name[0], 128, offset);
            loadData(data, animationHeaders[i].duration, offset);
            loadData(data, animationHeaders[i].ticksPerSecond, offset);

            animationBoneHeaders[i].resize(header.numberBones);
            positionKeys[i].resize(header.numberBones);
            rotationKeys[i].resize(header.numberBones);
            scalingKeys[i].resize(header.numberBones);    
            for(uint32_t j = 0; j < animationBoneHeaders[i].size(); j++){
                loadString(data, &animationBoneHeaders[i][j].name[0], 128, offset);
                loadData(data, animationBoneHeaders[i][j].numPositionKeys, offset);
                loadData(data, animationBoneHeaders[i][j].numRotationKeys, offset);
                loadData(data, animationBoneHeaders[i][j].numScalingKeys, offset);

                positionKeys[i][j].resize(animationBoneHeaders[i][j].numPositionKeys);
                for(uint32_t k = 0; k < positionKeys[i][j].size(); k++){
                    loadData(data, positionKeys[i][j][k].time, offset);
                    loadData(data, positionKeys[i][j][k].position, offset);
                }

                rotationKeys[i][j].resize(animationBoneHeaders[i][j].numRotationKeys);
                for(uint32_t k = 0; k < rotationKeys[i][j].size(); k++){
                    loadData(data, rotationKeys[i][j][k].time, offset);
                    loadData(data, rotationKeys[i][j][k].rotation, offset);
                }

                scalingKeys[i][j].resize(animationBoneHeaders[i][j].numScalingKeys);
                for(uint32_t k = 0; k < scalingKeys[i][j].size(); k++){
                    loadData(data, scalingKeys[i][j][k].time, offset);
                    loadData(data, scalingKeys[i][j][k].scaling, offset);
                }
            }
        }       

        asset.set<Rx::Component::SkeletonMeshArray>(meshArray);
        asset.set<Rx::Component::TextureMaterialBuffer>(textureMaterialBuffer);
        asset.set<Rx::Component::IndirectBuffer>(indirectBuffer);
        asset.set<Rx::Component::TextureArray>(textureArray);

        asset.add<Rx::Component::VkSkeletonMeshArray>();
        asset.add<Rx::Component::VkTextureMaterialBuffer>();
        
        Rx::Component::VkIndirectBuffer vkIndirectBuffer;
        vkIndirectBuffer.maxNumberCommands = header.numberMeshes;
        vkIndirectBuffer.numberCommands = 0;
        asset.set<Rx::Component::VkIndirectBuffer>(vkIndirectBuffer);

        asset.add<Rx::Component::VkTextureArray>();

        Rx::Component::VkTransformBuffer transformBuffer;
        transformBuffer.maxNumberTransforms = maxNumberInstances;
        transformBuffer.numberTransforms = 0;
        asset.set<Rx::Component::VkTransformBuffer>(transformBuffer);

        Rx::Component::VkSkeletonArrayBuffer vkSkeletonArrayBuffer;
        vkSkeletonArrayBuffer.maxNumberSkeletons = maxNumberInstances;
        vkSkeletonArrayBuffer.numberSkeletons = 0;
        asset.set<Rx::Component::VkSkeletonArrayBuffer>(vkSkeletonArrayBuffer);

        
        asset.add<Rx::Component::VkSkeletonModelDescriptorSet>();
        asset.add<Rx::Component::VkShadowSkeletonModelDescriptorSet>();
        asset.add<Rx::ShouldBeUpdated>();

        flecs::entity animationPrefab = world.prefab((assetName + "_Animation").c_str());


        Rx::Component::AnimationMap map;
        for(uint32_t i = 0; i < header.numberAnimations; i++){
            std::shared_ptr<Component::AnimationClip> animationClip = std::make_shared<Component::AnimationClip>();
            animationClip->duration = animationHeaders[i].duration;
            animationClip->ticksPerSecond = animationHeaders[i].ticksPerSecond;
            animationClip->durationInSeconds = animationHeaders[i].duration / animationHeaders[i].ticksPerSecond;
            std::vector<Component::AnimationBone> animationBones;
            uint32_t nodeIndex = 0;
            createAnimationBones(world, nodes, nodeNames, nodeIndex, i, animationBones, positionKeys, rotationKeys, scalingKeys);
            createKeyFrames(animationBones, animationClip->keyFrames, animationClip->keyFrameTimes, 0.1f);
            map.animations[animationHeaders[i].name] = std::move(animationClip);
        }
        for(uint32_t i = 0; i < nodeNames.size(); i++){
            map.boneIndices[nodeNames[i].name]  = nodes[i].boneIndex;
        }
        animationPrefab.set<Rx::Component::AnimationMap>(map);

        asset.set<Rx::Component::Skeleton>({nodes, std::vector<Component::VkNode>(), nodeNames,  animationPrefab});

        asset.add<Rx::Component::VkSkeleton>();

        Rx::Component::VkKeyFrameArrayBuffer keyFrameArrayBuffer;
        keyFrameArrayBuffer.maxNumberKeyFrameArrays = maxNumberInstances;
        keyFrameArrayBuffer.numberKeyFrameArrays = 0;
        asset.set<Rx::Component::VkKeyFrameArrayBuffer>(keyFrameArrayBuffer);

        asset.add<Rx::Component::VkSkeletonModelCompDescriptorSet>();
        
        asset.add<Rx::Component::SkeletonEntityArray>();
        
        return asset;
    }




    void createNodeInstanceEntities(flecs::world& world, flecs::entity asset,  const std::string& skelInstanceName, uint32_t& nodeIndex, const Component::Skeleton& skeleton, const flecs::entity& parent, flecs::entity& instanceEntity, uint32_t skeletonIndex) {

        flecs::entity nodeInstanceEntity = world.entity((skelInstanceName + "_Node_" + skeleton.nodeNames[nodeIndex].name).c_str());
        nodeInstanceEntity.add(world.lookup("IsNodeOfChild"), asset);
        nodeInstanceEntity.add(world.lookup("IsNodeOf"), instanceEntity);
        nodeInstanceEntity.add(world.lookup("IsChildNodeOf"), parent);
        nodeInstanceEntity.add<Rx::Component::Transform>();
        nodeInstanceEntity.set<Rx::Component::NodeIndex>({nodeIndex});
        nodeInstanceEntity.set<Rx::Component::SkeletonIndex>({skeletonIndex});

		uint32_t currentNodeIndex = nodeIndex;
        for(int j = 0; j < skeleton.nodes[currentNodeIndex].numberChildren; j++){
            createNodeInstanceEntities(world, asset, skelInstanceName, ++nodeIndex, skeleton, nodeInstanceEntity, instanceEntity, skeletonIndex);
        }
    }


    flecs::entity createSkeletonModelInstance(flecs::world& world, const flecs::entity& asset, const std::string& name)
    {
        flecs::entity instanceEntity = world.entity(name.c_str());
        instanceEntity.add(world.lookup("IsSkeletonModelInstanceOf"), asset);

        auto& skeletonEntityArray = asset.get_mut<Rx::Component::SkeletonEntityArray>();
        uint32_t skeletonIndex = skeletonEntityArray.skeletons.size();
        skeletonEntityArray.skeletons.push_back(instanceEntity);

        const auto& skeleton = asset.get<Rx::Component::Skeleton>();
        instanceEntity.is_a(skeleton.animationPrefab);

		Rx::Component::KeyFrameBuffer keyFrameBuffer;
		keyFrameBuffer.keyFrames.resize(skeleton.nodes.size());
        instanceEntity.set<Rx::Component::KeyFrameBuffer>(keyFrameBuffer);

        flecs::entity rootNodeEntity = world.entity((name + "_RootNode").c_str());
        rootNodeEntity.add(world.lookup("IsNodeOfChild"), asset);
        rootNodeEntity.add(world.lookup("IsNodeOf"), instanceEntity);
        rootNodeEntity.add<Rx::Component::Transform>();
        rootNodeEntity.set<Rx::Component::NodeIndex>({0});
        rootNodeEntity.set<Rx::Component::SkeletonIndex>({skeletonIndex});

        uint32_t nodeIndex = 0;
        for (int j = 0; j < skeleton.nodes[0].numberChildren; j++) {
            createNodeInstanceEntities(world, asset, name, ++nodeIndex, skeleton, rootNodeEntity, instanceEntity, skeletonIndex);
        }

        return instanceEntity;
    }

}
}