#version 450
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


layout(binding = 1) uniform Model
{
    mat4 transform;
    mat4 normalTransform;
    vec4 albedo;
    vec4 metalRough;
    vec4 emissive;
}model;

layout(location = 0) in vec3 position;

void main() {
    vec4 worldPosition = model.transform * vec4(position, 1.0);

    uint lightIndex = gl_ViewIndex;
    gl_Position = light.light[lightIndex].lightSpaceMatrix * worldPosition; 
}