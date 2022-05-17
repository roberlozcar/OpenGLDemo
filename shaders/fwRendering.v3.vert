#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec2 inTexCoord;
in vec3 inNormal;
in vec3 inTangent;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

out vec3 color;
out vec3 pos;
out vec3 norm;
out vec2 texCoord;
out mat3 TangentMat;

void main()
{
	color = inColor;
	texCoord = inTexCoord;
	norm = (normal * vec4(inNormal, 0.0)).xyz;
	pos = (modelView * vec4(inPos, 1.0)).xyz;

	//Bump mapping
	vec3 Tangent = normalize(vec3(modelView * vec4(inTangent, 0.0)));
	vec3 Normal = normalize(vec3(modelView * vec4(inNormal, 0.0)));

	vec3 Bitangent = cross(Normal, Tangent);
	TangentMat = mat3(Tangent, Bitangent, Normal);
	
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
