#version 450
#define MAX_LIGHTS 2048

layout(location = 0) in vec2 fragCoord;
layout(location = 1) in vec3 fragNormal;

layout(binding = 1) uniform MaterialUBO {
    vec3 color;
} material_ubo;

struct PointLight {
    vec4 color;
    vec4 attenuation;
};

layout(std140, binding = 2) uniform LightsUBO {
    PointLight pointlights[MAX_LIGHTS];
    int pointLightCount;
} lights_ubo;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(normalize(lights_ubo.pointlights[0].color), lights_ubo.pointLightCount);
}