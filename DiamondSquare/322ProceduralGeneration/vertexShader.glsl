#version 430 core

layout(location=0) in vec4 squareCoords;
layout(location=1) in vec4 squareColors;
layout(location=2) in vec4 squareNormals;

uniform mat4 projMat;
uniform mat4 modelViewMat;

flat out vec4 colorsExport;
flat out vec4 normals;

void main(void)
{
   gl_Position = projMat * modelViewMat * squareCoords;
   colorsExport = squareColors;
   normals = squareNormals;
}