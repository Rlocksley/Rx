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

layout(location = 0) in vec3 inPosition;

void main() {

    TransformInstance instance = modelBuffer.instances[gl_InstanceIndex];

    vec4 worldPosition = instance.transform * vec4(inPosition, 1.0);

    uint lightIndex = gl_ViewIndex;
    gl_Position = light.light[lightIndex].lightSpaceMatrix * worldPosition;
}
