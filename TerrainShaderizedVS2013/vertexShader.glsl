#version 430 core

#define SQUARE_VERTICES 0
#define SKY_VERTICES 1

layout(location=0) in vec4 squareCoords;
layout(location=1) in vec3 squareNormals;
layout(location=2) in vec2 squareTexCoords;

struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};

uniform mat3 normalMat;

uniform mat4 projMat;
uniform mat4 modelViewMat;

smooth out vec4 colorsExport;

out vec2 texCoordsExport;

uniform Material terrainFandB;

uniform Light light0;

uniform vec4 globAmb;

uniform vec4 skyColour;

uniform uint Object;

void main(void)
{
	if (Object == 0 ){
		texCoordsExport = squareTexCoords;
		gl_Position = projMat * modelViewMat * squareCoords;
		vec3 normal = normalize(normalMat * squareNormals);
		vec3 lightDirection = normalize(vec3(light0.coords));
		colorsExport =globAmb * terrainFandB.ambRefl* max(dot(normal, lightDirection), 0.0f);
	}
	if (Object == 1){
		texCoordsExport = squareTexCoords;
		gl_Position = projMat * modelViewMat * squareCoords;
		colorsExport = skyColour;
	}
}