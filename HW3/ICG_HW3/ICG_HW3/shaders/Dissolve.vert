#version 330 core

// Advanced:
// Implement Dissolve effect

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 camera;

out vec2 texCoord;
out vec3 normal;
out vec4 worldPos;
out float xPos;

void main()
{
	worldPos = M * vec4(aPos, 1.0);
	gl_Position = P * V * worldPos;
	texCoord = aTexCoord;
	mat4 normal_transform = transpose(inverse(M));
	normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);
	xPos = vec4(aPos, 1.0).x;
}