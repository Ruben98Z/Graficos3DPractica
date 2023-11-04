#version 330 core

out vec4 outColor;
in vec2 vtextcoord;
uniform sampler2D colorTex;


void main()
{
	vec4 blackColor = vec4(0,0,0,1);
	vec4 finalColor = texture(colorTex, vtextcoord);

	if( finalColor != blackColor)
		outColor = finalColor;
	else
		discard;
	
}
