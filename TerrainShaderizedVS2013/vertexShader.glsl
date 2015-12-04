#version 430 core

layout(location=0) in vec4 squareCoords;
layout(location=1) in vec4 squareColors;

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

uniform mat4 projMat;
uniform mat4 modelViewMat;

flat out vec4 colorsExport;

uniform Material terrainFandB;

uniform Light light0;

uniform vec4 globAmb;

void main(void)
{
   gl_Position = projMat * modelViewMat * squareCoords;
   colorsExport = globAmb * terrainFandB.ambRefl;
}