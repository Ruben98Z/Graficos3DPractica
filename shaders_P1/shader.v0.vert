#version 330 core

in vec3 inPos;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 modelViewProj;

void main()
{
	mat4 model = mat4(1.);
	mat4 proj = mat4(0.);
	mat4 view = mat4(1.);
	view[3].z = -3.;  

	proj[0].x = 1.73205080;
	proj[1].y = 1.73205080;
	proj[2].z = -1.2;
	proj[2].w = -1.0;
	proj[3].z = -2.2;
	
	gl_Position = proj * view * model * vec4(inPos, 1.0);

}
