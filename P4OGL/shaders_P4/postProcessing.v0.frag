#version 460 core

in vec2 texCoord;

layout (binding = 0) uniform sampler2D colorTex;

layout (location = 0) out vec4 outColor;

void main()
{
	//(gl_FragCoord)
	outColor = vec4(texture(colorTex, texCoord).xyz, 0.6); // Multplica la fuente por 0.6
}