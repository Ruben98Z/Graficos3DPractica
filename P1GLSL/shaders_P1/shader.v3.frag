#version 330 core


in vec3 vcolor;
out vec4 outColor;


void main()
{
	outColor = vec4(vcolor, 1.);
}
