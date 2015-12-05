#version 430 core

#define SQUARE_VERTICES 0
#define SKY_VERTICES 1

smooth in vec4 colorsExport;

out vec4 colorsOut;

in vec2 texCoordsExport;

uniform sampler2D tex;

uniform uint Object;


void main(void)
{
	if (Object == 0){
		vec4 fieldTexColor = texture(tex, texCoordsExport);
		colorsOut = fieldTexColor * colorsExport;
	}
	if (Object == 1){
		colorsOut = colorsExport;
	}
	if (Object == 2){
		colorsOut = colorsExport;
	}
}