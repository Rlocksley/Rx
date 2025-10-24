#version 450


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

layout(binding = 5) uniform Model
{
    mat4 transform;
    mat4 normalTransform;
    vec4 albedo;
    vec4 metalRough;
    vec4 emissive;
}model;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;


layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec3 albedo;
layout(location = 3) out vec3 metalRough;
layout(location = 4) out vec3 emissive;
layout(location = 5) out vec4 outFragPosLightSpace[16];

void main() {
    vec4 worldPosition = model.transform * vec4(position, 1.0);
    gl_Position = eye.projView * worldPosition;
    fragPosition = worldPosition.xyz / worldPosition.w;

    fragNormal = normalize(mat3(model.normalTransform) * normal);

    albedo = model.albedo.rgb;
    metalRough = model.metalRough.rgb;
    emissive = model.emissive.rgb;

    // Calculate fragment position in light space for each shadow spot light
    int numShadowLights = shadowSpotLightBuffer.numberShadowSpotLights.x;
    for (int i = 0; i < numShadowLights && i < 16; ++i) {
        outFragPosLightSpace[i] = shadowSpotLightBuffer.lights[i].lightSpaceMatrix * worldPosition;
    }
}