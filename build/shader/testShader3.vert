#version 450

layout(binding = 0) uniform Eye
{
    mat4 projView;
}eye;

layout(binding = 1) uniform Model
{
    mat4 transform;
}model;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;

layout(location = 0) out vec4 colorOut;

void main() {
    gl_Position = eye.projView * model.transform * vec4(position, 1.0);

    colorOut = color;
}