#version 460 core

layout (location = 0) in vec2 inPos;	
out vec2 texCoord;

void main()
{
	texCoord = (inPos + vec2(1)) * 0.5;
	gl_Position = vec4(inPos, 0, 1);
}
