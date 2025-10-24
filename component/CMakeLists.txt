cmake_minimum_required(VERSION 3.16.3)

project(rx)

add_library(rx_component STATIC
Transform.hpp
Transform.cpp
ColorMesh.hpp
ColorMesh.cpp
VkColorMesh.hpp
VkColorMesh.cpp
VkColorModelBuffer.hpp
VkColorModelBuffer.cpp
VkColorModelDescriptorSet.hpp
VkColorModelDescriptorSet.cpp
FlyingCamera.hpp
FlyingCamera.cpp
Velocity.hpp
Velocity.cpp
ColorMeshArray.hpp
ColorMeshArray.cpp
VkColorModelArrayDescriptorSet.hpp
VkColorModelArrayDescriptorSet.cpp
VkShadowColorModelArrayDescriptorSet.hpp
VkShadowColorModelArrayDescriptorSet.cpp
VkColorMeshArray.hpp
VkColorMeshArray.cpp
VkInstancedColorModelBuffer.hpp
VkInstancedColorModelBuffer.cpp
VkIndirectBuffer.hpp
VkIndirectBuffer.cpp
VkInstancedColorModelDescriptorSet.hpp
VkInstancedColorModelDescriptorSet.cpp
TextureArray.hpp
TextureArray.cpp
VkTextureArray.hpp
VkTextureArray.cpp
TextureMeshArray.hpp
TextureMeshArray.cpp
VkTextureMeshArray.hpp
VkTextureMeshArray.cpp
VkTransformBuffer.hpp
VkTransformBuffer.cpp
VkTextureMaterialBuffer.hpp
VkTextureMaterialBuffer.cpp
VkTextureModelDescriptorSet.hpp
VkTextureModelDescriptorSet.cpp
VkShadowTextureModelDescriptorSet.hpp
VkShadowTextureModelDescriptorSet.cpp
AnimationBone.hpp
AnimationBone.cpp
AnimationClip.hpp
AnimationClip.cpp
AnimationStateMachine.hpp
AnimationStateMachine.cpp
SkeletonMeshArray.hpp
SkeletonMeshArray.cpp
VkSkeletonMeshArray.hpp
VkSkeletonMeshArray.cpp
VkSkeletonArrayBuffer.hpp
VkSkeletonArrayBuffer.cpp
VkSkeletonModelDescriptorSet.hpp
VkSkeletonModelDescriptorSet.cpp
VkShadowSkeletonModelDescriptorSet.hpp
VkShadowSkeletonModelDescriptorSet.cpp
KeyFrameBuffer.hpp
KeyFrameBuffer.cpp
Skeleton.hpp
Skeleton.cpp
VkSkeleton.hpp
VkSkeleton.cpp
VkKeyFrameArrayBuffer.hpp
VkKeyFrameArrayBuffer.cpp
VkSkeletonModelCompDescriptorSet.hpp
VkSkeletonModelCompDescriptorSet.cpp
CharacterCapsule.hpp
CharacterCapsule.cpp
CharacterController.hpp
CharacterController.cpp
CharacterCamera.hpp
CharacterCamera.cpp
ShadowSpotLight.hpp
ShadowSpotLight.cpp
VkShadowColorModelDescriptorSet.hpp
VkShadowColorModelDescriptorSet.cpp
)

target_include_directories(rx_component PUBLIC ../core ../flecs)
target_link_libraries(rx_component PUBLIC
    # External dependencies
    Vulkan::Vulkan  # The imported target for Vulkan
    glm::glm        # The imported target for GLM
    glfw            # The imported target for GLFW
    rx_flecs
    rx_core
)