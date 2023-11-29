#version 330 core

out vec4 outColor;
in vec2 vtextcoord;
uniform sampler2D colorTex;


void main()
{
	outColor = texture(colorTex, vtextcoord);
	//outColor = vec4(vtextcoord,1,1);
}
