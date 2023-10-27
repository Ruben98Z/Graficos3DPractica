#version 330 core

in vec3 inPos;
in vec3 inColor;

out vec3 vcolor;

uniform mat4 modelViewProj;

void main()
{
	vcolor = inColor;
	gl_Position = modelViewProj * vec4(inPos, 1.0);
}
