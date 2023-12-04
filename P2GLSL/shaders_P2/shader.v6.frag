#version 330 core

out vec4 outColor;

in vec3 vnorm;
in vec3 vpos;

uniform mat4 view;

in vec2 vtc;
uniform sampler2D colorTex;
uniform sampler2D specularTex;
uniform sampler2D emiTex;

//Variables uniformes de la fuente de luz
vec3 Ia = vec3(0.1);
vec3 Il = vec3(1);

// Dirección de la luz
vec3 lightDirection = (view * vec4(vec3(-1, 0, 0), 0)).xyz; 

vec3 L;

//Variables uniformes del objeto
vec3 Ka = vec3(1,0,0);
vec3 Kd = vec3(1,0,0);
vec3 Ks = vec3(1);
vec3 Ke = vec3(0);
float n = 100.0;

//Variables locales al shader
vec3 N;
vec3 p;

vec3 shade()
{
	vec3 c = vec3(0);

	//Amb
	c+= Ia*Ka;

	//Diff
	L= normalize(-lightDirection);
	vec3 Id = Il*Kd* max(dot(N,L),0);
	c += Id;

	//Spc
	vec3 R = reflect(-L, N);
	vec3 V = normalize(-p);
	vec3 Is = Il*Ks* pow(max(dot(R,V),0),n);
	c += Is; 

	//Emi
	c += Ke;

	return c;
}


void main()
{
	N = vnorm;
	p = vpos;
	Kd = texture(colorTex, vtc).rgb;
	Ka = Kd;
	Ks = texture(specularTex, vtc).rgb;
	Ke = texture(emiTex, vtc).rgb;
	outColor = vec4(shade(), 1.0);   
}
