#version 330 core


in vec3 vcolor;
in vec3 vnormal;
out vec4 outColor;



void main()
{
	outColor = vec4(abs(vnormal), 1.);
}
