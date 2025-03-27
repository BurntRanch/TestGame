#version 450

layout(location = 0) in vec2 fragCoord;
layout(location = 1) in vec3 fragNormal;

layout(binding = 1) uniform MaterialUBO {
    vec3 color;
} material_ubo;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(material_ubo.color, 1.0);
}