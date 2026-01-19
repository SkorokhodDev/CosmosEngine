#version 450

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
    mat4 transform; // projection * view * model
    mat4 normalMatrix;
} push;

const vec3 DIRECTIONAL_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0)); //location
const float AMBIENT = 0.02;

void main() {
   // gl_Position = vec4(push.transform * positions + push.offset, 0.0, 1.0);
    gl_Position = push.transform * vec4(positions, 1.0); // 1 = homogeneous coordinates

    //only works correctly if scale is uniform (=1 in all axes)
    //vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * normal);
    //calculating the inverse in a shader can be expensive and should be avoided
    //mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));

    vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);
    float lightIntensity = AMBIENT + max(dot(normalWorldSpace, DIRECTIONAL_TO_LIGHT), 0.0);

    fragColor = lightIntensity * color;
}