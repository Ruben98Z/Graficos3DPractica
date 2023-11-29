#version 330 core


in vec3 vcolor;
in vec3 vnormal;
out vec4 outColor;

//uniform mat4 modelView;


void main()
{
	//outColor = modelView * vec4(vnormal,0.);
	outColor = vec4(vnormal,0.);
}
