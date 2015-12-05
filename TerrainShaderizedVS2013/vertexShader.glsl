#version 430 core

#define SQUARE_VERTICES 0
#define SKY_VERTICES 1

layout(location=0) in vec4 squareCoords;
layout(location=1) in vec3 squareNormals;
layout(location=2) in vec2 squareTexCoords;
layout(location=3) in vec4 squareColors;

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

uniform uint Object;

uniform mat4 treeTranslate;

void main(void)
{
	if (Object == 0 ){
		texCoordsExport = squareTexCoords*2049;
		gl_Position = projMat * modelViewMat * squareCoords;
		vec3 normal = normalize(normalMat * squareNormals);
		vec3 lightDirection = normalize(vec3(light0.coords));
		colorsExport =globAmb * terrainFandB.ambRefl* max(dot(normal, lightDirection), 0.0f);
	}
	if (Object == 1){
		gl_Position = projMat * modelViewMat *treeTranslate*squareCoords;
		colorsExport = squareColors;
	}
	if (Object == 2){
<<<<<<< HEAD
		gl_Position = projMat * modelViewMat *treeTranslate*squareCoords;
=======
		gl_Position = projMat * modelViewMat *treeTranslate* squareCoords;
>>>>>>> parent of bcab911... Tree place well
		colorsExport = squareColors;
	}
}