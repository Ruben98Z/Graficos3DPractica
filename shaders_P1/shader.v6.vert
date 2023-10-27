#version 330 core

in vec3 inPos;
in vec2 inTexCoord;
out vec2 vtextcoord;

uniform mat4 modelViewProj;

void main()
{
	vtextcoord = inTexCoord;
	gl_Position = modelViewProj * vec4(inPos, 1.0);
}
