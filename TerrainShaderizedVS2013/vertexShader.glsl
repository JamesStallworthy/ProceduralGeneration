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

smooth out float yAxis;

out vec2 texCoordsExport;

uniform Material terrainFandB;

uniform Light light0;

uniform vec4 globAmb;

uniform vec4 skyColour;

uniform uint Object;

uniform mat4 treeTranslate;

uniform mat4 skyTranslate;

void main(void)
{
	if (Object == 0 ){
		texCoordsExport = squareTexCoords;
		gl_Position = projMat * modelViewMat * squareCoords;
		yAxis = squareCoords.y;
		vec3 normal = normalize(normalMat * squareNormals);
		vec3 lightDirection = normalize(vec3(light0.coords));
		colorsExport =max(dot(normal, lightDirection), 0.0f) * (light0.difCols * terrainFandB.difRefl); 
	}
	if (Object == 1){
		texCoordsExport = squareTexCoords;
		gl_Position = projMat * modelViewMat * squareCoords;
		colorsExport = skyColour;
	}
	if (Object == 2){
		gl_Position = projMat * modelViewMat *treeTranslate* squareCoords;
		colorsExport = squareColors/1.3;
	}

	if (Object == 3){
		gl_Position = projMat * modelViewMat *skyTranslate * squareCoords;
		if (squareCoords.y>50){
			colorsExport = vec4(0.9f,0.9f,1.0f,1.0f);
		}
		else{
			colorsExport = vec4(1.0f,1.0f,1.0f,0.0f);
		}
	}
}