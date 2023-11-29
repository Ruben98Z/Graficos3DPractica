#version 330 core

in vec3 inPos;	
in vec2 inTexCoord;	

in vec3 inNormal;
uniform mat4 modelView;
uniform mat4 normal;
uniform mat4 modelViewProj;
out vec3 vnorm;
out vec3 vpos;
out vec2 vtc; //coordenadas de textura






void main()
{
	vnorm = normalize((normal * vec4(inNormal, 0)).xyz);
	vpos = (modelView * vec4(inPos, 1)).xyz;
	vtc = inTexCoord;



	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
