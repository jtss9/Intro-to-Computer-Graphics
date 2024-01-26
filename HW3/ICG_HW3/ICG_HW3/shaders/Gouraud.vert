#version 330 core

// TODO:
// Implement Gouraud shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 camera;

out vec2 texCoord;
out vec3 ambient;
out vec3 diffuse;
out vec3 specular;

vec4 worldPos;

void main()
{
	worldPos = M * vec4(aPos, 1.0);
	gl_Position = P * V * worldPos;
	texCoord = aTexCoord;
	mat4 normal_transform = transpose(inverse(M));
	vec3 normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);
	vec3 viewPos = camera;

	vec3 lightPos = vec3(10, 10, 10);
	vec3 Ka = vec3(1.0, 1.0, 1.0);
	vec3 Kd = vec3(1.0, 1.0, 1.0);
	vec3 Ks = vec3(0.7, 0.7, 0.7);
	float a = 10.5;
	vec3 La = vec3(0.2, 0.2, 0.2);
	vec3 Ld = vec3(0.8, 0.8, 0.8);
	vec3 Ls = vec3(0.5, 0.5, 0.5);

	vec3 N = normalize(normal);
	vec3 L = normalize(lightPos - worldPos.xyz);
	vec3 Vi = normalize(viewPos - vec3(worldPos));
	vec3 R = normalize(reflect(-lightPos, normal));

	ambient = La * Ka;
	diffuse = Ld * Kd * max(0.0, dot(L, N));
	specular = Ls * Ks * pow(max(0.0, dot(Vi, R)), a);
}