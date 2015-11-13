#version 430 core

flat in vec4 colorsExport;

out vec4 colorsOut;

uniform vec4 Colour;

void main(void)
{
	colorsOut = Colour;
}
