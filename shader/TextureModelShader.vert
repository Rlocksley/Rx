#version 460
#extension GL_ARB_shader_draw_parameters : require


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

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 outTexCoords;
layout(location = 3) out flat int glDrawID;
layout(location = 4) out vec4 outFragPosLightSpace[16];

void main() {

    TransformInstance instance = modelBuffer.instances[gl_InstanceIndex];

    vec4 worldPosition = instance.transform * vec4(inPosition, 1.0);

    gl_Position = eye.projView * worldPosition;
 
    fragPosition = vec3(worldPosition);

    fragNormal = mat3(instance.normalTransform) * inNormal;

    outTexCoords = inTexCoords;

    glDrawID = int(gl_DrawID);

         // Calculate fragment position in light space for each shadow spot light
    int numShadowLights = shadowSpotLightBuffer.numberShadowSpotLights.x;
    for (int i = 0; i < numShadowLights && i < 16; ++i) {
        outFragPosLightSpace[i] = shadowSpotLightBuffer.lights[i].lightSpaceMatrix * worldPosition;
    }

}
