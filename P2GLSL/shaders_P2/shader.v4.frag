#version 330 core

out vec4 outColor;

in vec3 vnorm;
in vec3 vpos;

uniform mat4 view;

in vec2 vtc;
uniform sampler2D colorTex;
uniform sampler2D specularTex;
uniform sampler2D emiTex;

// Definir un vector para la posición actual de la luz
vec3 offset = vec3(6.0, 0.0, 6.0);
//Fuente de luz 1
//Variables uniformes de la fuente de luz
vec3 Il = vec3(0, 0, 0.7);  
vec3 pl = (view * vec4(offset, 0)).xyz;

// Fuente de luz 2
vec3 Il2 = vec3(0.7, 0, 0); 
vec3 pl2 = (view * vec4(-offset, 0)).xyz; 


vec3 Ia = vec3(0.5); // Intensidad Ambiental
vec3 L, L2;


//Variables uniformes del objeto
vec3 Ka = vec3(1,0,0); // Coeficiente de reflexión ambiental
vec3 Kd = vec3(1,0,0); // Coeficiente de reflexión difusa
vec3 Ks = vec3(1);	   // Coeficiente de reflexión especular
vec3 Ke = vec3(0);     // Coeficiente de reflexión emisiva

float n = 100.0;

//Variables locales al shader
vec3 N;
vec3 p;

vec3 shade1()
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

	//Emi
	c += Ke;

	return c;
}

vec3 shade2()
{
    vec3 c = vec3(0);

    // Ambiente
    c += Ia * Ka;

    // Difuso 2
    L2 = normalize(pl2-p);
    c += Il2 * Kd * max(dot(N, L2), 0);

    // Especular 2
    vec3 V = normalize(-p);
    vec3 R2 = reflect(-L2, N);
    c += Il2 * Ks * pow(max(dot(R2, V), 0), n);

    // Emisivo
    c += Ke;

    return c;
}


vec3 shade()
{
	vec3 c = vec3(0);
	c += shade1();
	c += shade2();
	return  c;
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
