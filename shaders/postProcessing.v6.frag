#version 330 core

//Color de salida
out vec4 outColor;

//Variables Variantes
in vec2 texCoord;

//Textura
uniform sampler2D DiffTex;
uniform sampler2D PosTex;
uniform sampler2D NormalTex;
uniform sampler2D EmiTex;

//Depth
uniform sampler2D depthTex;
uniform float f;
uniform float n;
uniform float offset;

#define MASK_SIZE 25u 
uniform vec2 texIdx[MASK_SIZE];
uniform float mask[MASK_SIZE];

uniform float focalDistance;				//donde quiero ver nitido
uniform float maxDistanceFactor;			//donde quiero el max desenfoque

//Propiedades de la luz
vec3 Ia = vec3 (0.3);
vec3 Id = vec3 (1.0);
vec3 Is = vec3 (0.7);
vec3 lpos = vec3 (0.0); 
float alpha = 500.0;

//Dirección en la luz focal
vec3 Dir = normalize(vec3(0.0, 0.0, -1.0));
float cutoff = 0.85;
float penumbra = 0.90;

//direccional
vec3 D = vec3(0.0, -1.0, -1.0);
uniform int typeOfLight; //0 = puntual, 1 = direccional, 2 = focal
float fdir = 1.0;
float fdist = 1.0;


vec3 shade(vec4 diff,vec2 Coor)
{
	vec3 Pos=texture(PosTex,Coor).rgb;
	vec3 N=texture(NormalTex,Coor).rgb;

	//Atenuación
	if(typeOfLight != 1){			//si es luz direccional no puede haer atenuación con la distancia, ya que la luz no está en un punto
		float d0 = 50.0;
		float dmin = 15.0;
		float dmax = 60.0;
		float d = distance(lpos, Pos);

		//funcion ventana
		float f = pow(1 - (d / dmax), 4.0);
		float fwin = pow(max(f, 0.0), 2.0);

		fdist = pow((d0 / max(d, dmin)), 2.0) * fwin;
	}

	vec3 c = vec3(0.0);
	c = Ia * diff.rgb;

	vec3 L = normalize (lpos - Pos);

	//Distintos tipos de luz
	if(typeOfLight == 2){			//luz focal
		float t = clamp (((dot(-L,Dir) - cutoff) / (penumbra - cutoff)), 0.0, 1.0);
		fdir = smoothstep(0, 1, t);
	}

	if(typeOfLight == 1){			//Luz direccional
		L = -D;
	}

	vec3 diffuse = Id * diff.rgb * dot (L,N) * fdir * fdist;
	c += clamp(diffuse, 0.0, 1.0);
	
	vec3 V = normalize (-Pos);
	vec3 R = normalize (reflect (-L,N));
	float factor = max (dot (R,V), 0.01);
	vec3 specular = Is*diff.a*pow(factor,alpha) * fdir * fdist;
	c += clamp(specular, 0.0, 1.0);

	c+=texture(EmiTex,Coor).rgb;
	
	return c;
}

void main()
{
	float zdepth=-n * f / (f+texture(depthTex,texCoord).x * (n-f));
	// dof->[0.1]
	// cuando dof vale 1, obtenemos lo mismo de antes
	// cuando dof vale 0, iidx valdra las coordenadas del texel (sin desenfoque)
	// cuando dof vale 0.5 desenfoca menos

	// Sería más rápido utilizar una variable uniform el tamaño de la textura. 
	vec2 ts = vec2(1.0) / vec2 (textureSize (DiffTex,0)); 

	//float dof = abs(texture(posTex,texCoord).z -focalDistance) * maxDistanceFactor; 
	float dof = abs(zdepth - focalDistance) * maxDistanceFactor; //version depth
	dof = clamp (dof, 0.0, 1.0);
	dof *= dof; 

	// Aplico la mascara de convolucion
	vec4 color1 = vec4 (0.0); 
	vec4 diff= color1;

	//Distorsion
	vec2 Coor = texCoord;
    Coor.x += sin(Coor.y * 50 + offset) / 100;

    for (uint i = 0u; i < MASK_SIZE; i++) { 
        vec2 iidx = Coor + ts*texIdx[i]*dof;
        diff += texture(DiffTex, iidx) * mask[i];
    } 

	vec4 color=vec4(shade(diff,Coor),1.0);
	
	// Desaturacion
	vec4 grey=vec4(dot(color,vec4(0.707,0.0,0.707,0.0)))/2;

	// Mezcla de post-procesos
    vec4 mezcla=mix(grey,color,0.2);
	
	outColor=mezcla;
 	
}