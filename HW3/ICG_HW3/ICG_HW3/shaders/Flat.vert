#version 330 core

// TODO:
// Implement Flat shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 camera;

out VS_OUT {
	vec2 texCoord;
	vec3 normal;
	vec4 worldPos;
} vs_out;


void main()
{
	vs_out.worldPos = M * vec4(aPos, 1.0);
	gl_Position = P * V * vs_out.worldPos;
	vs_out.texCoord = aTexCoord;
	mat4 normal_transform = transpose(inverse(M));
	vs_out.normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);
}