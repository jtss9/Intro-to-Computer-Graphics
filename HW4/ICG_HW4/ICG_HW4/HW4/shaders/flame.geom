#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

in VS_OUT {
	vec3 normal;
	vec3 fragpos;
	vec2 texCoord;
	vec3 viewDir;
} gs_in[];

out GS_OUT {
	vec3 fragposGS;
	vec3 normalGS;
	vec2 texCoordGS;
	vec3 viewDirGS;
} gs_out;

uniform mat4 P;
uniform float time;
uniform float auraWidth;
float flameHeight = 0.05;

void main(){
	vec3 flameDir = normalize(gs_in[0].viewDir);
	vec3 flameBase = gl_in[0].gl_Position.xyz + normalize(flameHeight * flameDir);

	for (int i = 0; i < gl_in.length(); ++i) {
		float offset = sin(time*3.0+auraWidth*10.0 + i) * 0.1 + cos(time*3.0+auraWidth*10.0+i)*0.1 + auraWidth;
		vec3 dir = normalize(gs_in[i].normal);
		vec3 flameTip = gl_in[i].gl_Position.xyz + 0.1*normalize(flameHeight * dir)+0.5*offset;
		vec3 flameTip2 = gl_in[i].gl_Position.xyz - 0.1*normalize(flameHeight * dir)-0.5*offset;
		
		gl_Position = P * vec4(flameBase, 1.0);
		gs_out.fragposGS = flameBase;
		gs_out.texCoordGS = gs_in[i].texCoord;
		gs_out.normalGS = gs_in[i].normal;
		gs_out.viewDirGS = gs_in[i].viewDir;
		EmitVertex();

		gl_Position = P * vec4(flameTip, 1.0);
		gs_out.fragposGS = flameTip;
		gs_out.texCoordGS = gs_in[i].texCoord;
		gs_out.normalGS = gs_in[i].normal;
		gs_out.viewDirGS = gs_in[i].viewDir;
		EmitVertex();

		gl_Position = P * vec4(flameTip2, 1.0);
		gs_out.fragposGS = flameTip2;
		gs_out.texCoordGS = gs_in[i].texCoord;
		gs_out.normalGS = gs_in[i].normal;
		gs_out.viewDirGS = gs_in[i].viewDir;
		EmitVertex();
	}
	EndPrimitive();

}
