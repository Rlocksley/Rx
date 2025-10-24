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


struct ColorMeshInstance
{
    mat4 transform;
    mat4 normalTransform;
    vec4 albedo;
    vec4 metalRough;     
    vec4 emissive;
};

layout(binding = 1, std430) restrict readonly buffer ModelBuffer
{
    ColorMeshInstance instances[];
} modelBuffer;


layout(location = 0) in vec3 position;

void main() {
    vec4 worldPosition = modelBuffer.instances[gl_DrawID].transform * vec4(position, 1.0);

    uint lightIndex = gl_ViewIndex;
    gl_Position = light.light[lightIndex].lightSpaceMatrix * worldPosition; 
}