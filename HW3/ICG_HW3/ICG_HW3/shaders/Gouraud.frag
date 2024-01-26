#version 330 core

// TODO:
// Implement Gouraud shading

in vec2 texCoord;
in vec3 ambient;
in vec3 diffuse;
in vec3 specular;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	vec3 color = vec3(texture(ourTexture, texCoord));
	FragColor = vec4((ambient * color + diffuse * color + specular), 1.0);
}