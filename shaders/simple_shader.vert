#version 450

layout(location = 0) in vec2 positions;
layout(location = 1) in vec3 color;

//layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform PushConstants {
    vec2 offset;
    vec3 color;
} push;

void main() {
    gl_Position = vec4(positions + push.offset, 0.0, 1.0);
    //fragColor = push.color;
}