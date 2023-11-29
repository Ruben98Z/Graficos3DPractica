#version 330 core

in vec3 inPos;	
in vec3 inColor;

in vec3 inNormal;
uniform mat4 modelView;
uniform mat4 normal;
uniform mat4 modelViewProj;
out vec3 color;


//Variables uniformes de la fuente de luz
vec3 Ia = vec3(0.1);
vec3 Il = vec3(1);
vec3 pl = vec3(0); //Sistema de la camara
vec3 L;

//Variables uniformes del objeto
vec3 Ka = vec3(1,0,0);
vec3 Kd = vec3(1,0,0);
vec3 Ks = vec3(1);
float n = 10.0;

//Variables locales al shader
vec3 N;
vec3 p;


vec3 shade()
{
	vec3 c = vec3(0);

	//Amb
	c+= Ia*Ka;

	//Diff
	L= normalize(pl-p);
	c += Il*Kd* max(dot(N,L),0);

	//Spc
	vec3 R = reflect(-L, N);
	vec3 V = normalize(-p);

	c += Il*Ks* pow(max(dot(R,V),0),n); 

	return c;
}
void main()
{
	N = normalize((normal * vec4(inNormal, 0)).xyz);
	p = (modelView * vec4(inPos, 1)).xyz;

	color = shade();
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
