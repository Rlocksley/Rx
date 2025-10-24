#version 460
#extension GL_ARB_shader_draw_parameters : require
#extension GL_EXT_multiview : enable

struct Light
{
    vec4 position;
    vec4 direction;
    vec4 color;
    vec4 intensity;
    mat4 lightSpaceMatrix;
};

layout(binding = 0) uniform Lights
{
    ivec4 lightCount;
    Light light[16];
} light;



struct  TransformInstance
{
    mat4 transform;
    mat4 normalTransform;
};

layout(binding = 1, std430) restrict readonly buffer ModelBuffer
{
    TransformInstance instances[];
} modelBuffer;

struct NodeTransform{
    mat4 bone;
    mat4 local;
};

layout(binding = 2, std430) restrict readonly buffer Nodes
{
    NodeTransform nodes[];
} nodeBuffer;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in ivec4 boneIndices;
layout(location = 4) in vec4 boneWeights;
layout(location = 5) in int nodeIndex;

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
    
    uint lightIndex = gl_ViewIndex;
    gl_Position = light.light[lightIndex].lightSpaceMatrix * worldPosition;
}
