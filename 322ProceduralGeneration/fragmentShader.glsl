#version 430 core

flat in vec4 colorsExport;
flat in vec4 normals;

out vec4 colorsOut;

void main(void)
{
	colorsOut = colorsExport * normals;
}
