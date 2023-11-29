#version 330 core

in vec3 inPos;	
in vec3 inColor;

in vec3 inNormal;
uniform mat4 modelView;
uniform mat4 normal;
uniform mat4 modelViewProj;
out vec3 vnorm;
out vec3 vpos;
out vec3 vcolor;




void main()
{
	vnorm = normalize((normal * vec4(inNormal, 0)).xyz);
	vpos = (modelView * vec4(inPos, 1)).xyz;
	vcolor = inColor;

	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
