#version 330 core

in vec3 inPos;
in vec3 inColor;
in vec3 inNormal;

out vec3 vcolor;
out vec3 vnormal;

uniform mat4 modelViewProj;

void main()
{
	vnormal = inNormal;
	vcolor = inColor;
	gl_Position = modelViewProj * vec4(inPos, 1.0);
}
