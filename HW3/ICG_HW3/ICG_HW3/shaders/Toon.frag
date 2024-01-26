#version 330 core

// TODO:
// Implement Toon shading

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
	float a = 10.5;
	
	vec3 N = normalize(normal);
	vec3 L = normalize(lightPos - worldPos.xyz);
	vec3 Vi = normalize(viewPos - vec3(worldPos));
	vec3 R = normalize(reflect(-lightPos, normal));

	vec3 specular = vec3(0.5, 0.5, 0.5) * vec3(0.7, 0.7, 0.7) * pow(max(0.0, dot(Vi, R)), a);
	float cos = dot(N, L);
	vec4 intensity;
	if(cos < 0.4) intensity = vec4(0.21, 0.1, 0.07, 0.0);
	else if(specular.x > 0.02 || specular.y > 0.02 || specular.z > 0.02)	intensity = vec4(0.87, 0.77, 0.73, 0.0);
	else intensity = vec4(0.52, 0.33, 0.25, 0.0);

	FragColor = intensity;


}