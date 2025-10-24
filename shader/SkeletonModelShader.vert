#version 460
#extension GL_ARB_shader_draw_parameters : require

struct NodeTransform{
    mat4 bone;
    mat4 local;
};

layout(binding = 0) uniform Eye
{
    vec4 position;
    vec4 direction;
    mat4 projView;
} eye;

struct ShadowSpotLight
{
    vec4 position;
    vec4 direction;
    vec4 color;
    vec4 intensity;
    mat4 lightSpaceMatrix;
};

layout(binding = 3) uniform ShadowSpotLightBuffer
{
    ivec4 numberShadowSpotLights;
    ShadowSpotLight lights[16];
} shadowSpotLightBuffer;

struct  TransformInstance
{
    mat4 transform;
    mat4 normalTransform;
};

layout(binding = 5, std430) restrict readonly buffer ModelBuffer
{
    TransformInstance instances[];
} modelBuffer;

layout(binding = 7, std430) restrict readonly buffer Nodes
{
    NodeTransform nodes[];
} nodeBuffer;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in ivec4 boneIndices;
layout(location = 4) in vec4 boneWeights;
layout(location = 5) in int nodeIndex;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 outTexCoords;
layout(location = 3) out flat int glDrawID;
layout(location = 4) out vec4 outFragPosLightSpace[16];

void main() {
    
    mat4 skinMatrix;
    if(nodeIndex == -1){
        // For weighted vertices, compute normal skinning
        skinMatrix = boneWeights.x * nodeBuffer.nodes[gl_InstanceIndex * 256 + boneIndices.x].bone +
                     boneWeights.y * nodeBuffer.nodes[gl_InstanceIndex * 256 + boneIndices.y].bone +
                     boneWeights.z * nodeBuffer.nodes[gl_InstanceIndex * 256 + boneIndices.z].bone +
                     boneWeights.w * nodeBuffer.nodes[gl_InstanceIndex * 256 + boneIndices.w].bone;

    }else{
        // For not weighted vertices, transform with globalTransform of the bone (for example weapon in bone hand)
        skinMatrix = nodeBuffer.nodes[gl_InstanceIndex * 256 + nodeIndex].local;
    }
    
    // Apply the skinning transformation to vertex position.
    vec4 skinnedPosition = skinMatrix * vec4(inPosition, 1.0);
    vec4 worldPosition = modelBuffer.instances[gl_InstanceIndex].transform * skinnedPosition;
    gl_Position = eye.projView * worldPosition;
    fragPosition = worldPosition.xyz;
    
    // Compute skinning for normals.
    // Here we extract a 3x3 matrix from the skinMatrix.
    mat3 skinNormalMatrix = mat3(skinMatrix);
    vec3 skinnedNormal = normalize(skinNormalMatrix * inNormal);
    fragNormal = normalize(mat3(modelBuffer.instances[gl_InstanceIndex].normalTransform) * skinnedNormal);

    outTexCoords = inTexCoords;
    glDrawID = int(gl_DrawID);


         // Calculate fragment position in light space for each shadow spot light
    int numShadowLights = shadowSpotLightBuffer.numberShadowSpotLights.x;
    for (int i = 0; i < numShadowLights && i < 16; ++i) {
        outFragPosLightSpace[i] = shadowSpotLightBuffer.lights[i].lightSpaceMatrix * worldPosition;
    }
}