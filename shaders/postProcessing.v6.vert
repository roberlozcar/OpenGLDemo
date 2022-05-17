#version 330 core

// PROCESA EL SHADER DEL PLANO
layout (location = 0 ) in vec2 inPos;

out vec2 texCoord;

void main()
{
	// Calculamos las coordenadas de textura (rango 0 a 1)
	texCoord = 0.5*inPos + vec2(0.5);

	// Pasar a coordenadas normalizadas del plano de proyeccion
	gl_Position = vec4(inPos, 0.0f, 1.0f);
}

