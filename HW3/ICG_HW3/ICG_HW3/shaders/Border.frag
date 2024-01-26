#version 330 core

// Advanced:
// Implement Border effect

in vec2 texCoord;
in vec3 normal;
in vec4 worldPos;
in vec3 viewPos;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	vec3 color = vec3(texture(ourTexture, texCoord));
	vec3 lightPos = vec3(10, 10, 10);
	vec3 nNormal = normalize(normal);
	vec3 view_dir = normalize(viewPos - vec3(worldPos));

	float angle = dot(nNormal, view_dir);
	FragColor = 0.9*(1-angle) + angle*vec4(color, 1.0);
}