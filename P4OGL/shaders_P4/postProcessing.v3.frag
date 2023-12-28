#version 460 core

in vec2 texCoord;

layout (binding = 0) uniform sampler2D colorTex;
layout (binding = 1) uniform sampler2D zTex;

//const float focalDistance = -25.0;
//const float maxDistanceFactor = 1.0/5.0;

layout (location = 0) out vec4 outColor;

uniform float focalDistance;
uniform float maxDistanceFactor;
uniform float near;
uniform float far;

uniform float mask[25];
uniform float maskFactor;


#define MASK_SIZE 25u
const vec2 texIdx[MASK_SIZE] = vec2[](
	vec2(-2.0,2.0), vec2(-1.0,2.0), vec2(0.0,2.0), vec2(1.0,2.0), vec2(2.0,2.0),
	vec2(-2.0,1.0), vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0), vec2(2.0,1.0),
	vec2(-2.0,0.0), vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,0.0), vec2(2.0,0.0),
	vec2(-2.0,-1.0), vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0), vec2(2.0,-1.0),
	vec2(-2.0,-2.0), vec2(-1.0,-2.0), vec2(0.0,-2.0), vec2(1.0,-2.0), vec2(2.0,-2.0));

/*
const float maskFactor = float (1.0/65.0);
const float mask[MASK_SIZE] = float[](
	1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor,
	2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
	3.0*maskFactor, 4.0*maskFactor, 5.0*maskFactor,4.0*maskFactor, 3.0*maskFactor,
	2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
	1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor);
*/


void main()
{
	//Ser�a m�s r�pido utilizar una variable uniform el tama�o de la textura.
	vec2 ts = vec2(1.0) / vec2 (textureSize (colorTex,0));

	float dof = abs((-near*far/(far+(near-far)*texture(zTex, texCoord).r)) -focalDistance) * maxDistanceFactor;
	dof = clamp (dof, 0.0, 1.0);
	ts *= dof * dof;

	vec4 color = vec4 (0.0);
	for (uint i = 0u; i < MASK_SIZE; i++)
	{
		vec2 iidx = texCoord + ts * texIdx[i]*dof;
		color += texture(colorTex, iidx,0.0) * mask[i];
	}

	outColor = color;
}