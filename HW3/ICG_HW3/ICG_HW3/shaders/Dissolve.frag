#version 330 core

// Advanced:
// Implement Dissolve effect

in vec2 texCoord;
in vec3 normal;
in vec4 worldPos;
in float xPos;

uniform sampler2D ourTexture;
uniform float x_dissolve;

out vec4 FragColor;

void main()
{
	vec3 color = vec3(texture(ourTexture, texCoord));
	if(xPos >= x_dissolve)	FragColor = vec4(color, 1.0);
	else discard;

}