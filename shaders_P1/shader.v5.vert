#version 330 core

in vec3 inPos;
in vec3 inColor;
in vec3 inNormal;

out vec3 vcolor;
out vec3 vnormal;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

void main()
{
	//vnormal = (modelView * vec4(inNormal,0.)).xyz;
	vnormal = (normal * vec4(inNormal,0.)).xyz;
	vcolor = inColor;
	gl_Position = modelViewProj * vec4(inPos, 1.0);
}
