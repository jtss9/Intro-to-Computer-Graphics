#version 330 core

// TODO:
// Implement Flat shading

in vec2 texCoord;
in vec3 fragNormal;
in vec4 worldPos;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 camera;

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
	vec3 viewPos = camera;
	
	vec3 N = normalize(fragNormal);
	vec3 L = normalize(lightPos - worldPos.xyz);
	vec3 Vi = normalize(viewPos - vec3(worldPos));
	vec3 R = normalize(reflect(-lightPos, fragNormal));

	vec3 ambient = La * Ka;
	vec3 diffuse = Ld * Kd * max(0.0, dot(L, N));
	vec3 specular = Ls * Ks * pow(max(0.0, dot(Vi, R)), a);
	vec3 result = ambient*color + diffuse*color + specular;

	FragColor = vec4(result, 1.0);
}