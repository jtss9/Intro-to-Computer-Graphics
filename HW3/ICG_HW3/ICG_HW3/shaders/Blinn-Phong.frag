#version 330 core

// TODO:
// implement Blinn-Phong shading
in vec2 texCoord;
in vec3 normal;
in vec4 worldPos;
in vec3 viewPos;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	vec3 lightPos = vec3(10, 10, 10);
	vec3 Ka = vec3(1.0, 1.0, 1.0);
	vec3 Kd = vec3(1.0, 1.0, 1.0);
	vec3 Ks = vec3(0.7, 0.7, 0.7);
	float a = 10.5;
	vec3 La = vec3(0.2, 0.2, 0.2);
	vec3 Ld = vec3(0.8, 0.8, 0.8);
	vec3 Ls = vec3(0.5, 0.5, 0.5);

	vec3 color = vec3(texture(ourTexture, texCoord));

	// Ambient
	vec3 ambient = La * Ka * color;
	// Diffuse
	vec3 nNormal = normalize(normal);
	vec3 light = (normalize(vec4(lightPos, 1.0) - worldPos)).xyz;
	vec3 diffuse = Ld * Kd * color * max(0.0, dot(light, nNormal));
	// Specular
	vec3 view_dir = normalize(viewPos - vec3(worldPos));
	vec3 half_vec = normalize(lightPos + view_dir);
	vec3 specular = Ls * Ks * pow(max(0.0, dot(normal, half_vec)), a);

	FragColor = vec4((ambient + diffuse + specular), 1.0);
}