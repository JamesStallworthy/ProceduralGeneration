#version 430 core

smooth in vec4 colorsExport;

out vec4 colorsOut;

in vec2 texCoordsExport;

uniform sampler2D grassTex;

void main(void)
{
	vec4 fieldTexColor = texture(grassTex, texCoordsExport);
	colorsOut = fieldTexColor * colorsExport;
}