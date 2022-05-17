#version 330 core


layout (location=0) out vec4 Kd;
layout (location=1) out vec3 outPos;
layout (location=2) out vec3 outN;
layout (location=3) out vec3 Ke;
// Para sacar la profundidad
layout(location = 4) out float zdepth;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;
in mat3 TangentMat;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

void main()
{
	Kd = vec4 (texture(colorTex, texCoord).rgb,texture(specularTex, texCoord).r);
	Ke = texture(emiTex, texCoord).rgb;

	// Bump Mapping
	outPos=pos;
	vec3 N=texture(normalTex,texCoord).rgb;
	N = N * 2.0 - 1.0;
	outN = normalize (TangentMat*N);

	zdepth = pos.z;

}
