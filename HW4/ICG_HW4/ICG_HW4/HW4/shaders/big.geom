#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

in VS_OUT {
	vec3 normal;
	vec3 fragpos;
	vec2 texCoord;
} gs_in[];

uniform mat4 P;
uniform float auraWidth;

out vec3 fragposGS;
out vec3 normalGS;
out vec2 texCoordGS;

void main()
{	
	for (int i = 0; i < gl_in.length(); ++i) {
		fragposGS = gs_in[i].fragpos;
		texCoordGS = gs_in[i].texCoord;
		normalGS = normalize(gs_in[i].normal);

		vec4 originVertex = gl_in[i].gl_Position;
		vec4 auraVertex = originVertex + vec4(normalGS * auraWidth, 0.0);

		gl_Position = P * auraVertex;
		EmitVertex();
	}
	EndPrimitive();
}