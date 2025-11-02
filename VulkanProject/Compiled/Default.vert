#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (location = 0) out vec2 TexCoords;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec3 FragPos;
layout (location = 3) out vec3 camPos;

layout (set = 0, binding = 0) uniform uniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
	vec3 camerPos;
} ubo;

void main() {
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(aPos, 1.0);
	TexCoords = aTexCoords;
	Normal = aNormal;
	FragPos = vec3(ubo.model * vec4(aPos, 1.0));
	camPos = ubo.camerPos;
}