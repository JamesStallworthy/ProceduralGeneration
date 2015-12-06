#version 430 core

#define SQUARE_VERTICES 0
#define SKY_VERTICES 1

smooth in vec4 colorsExport;

out vec4 colorsOut;

in vec2 texCoordsExport;

smooth in float yAxis;

uniform sampler2D grassTex;

uniform sampler2D waterTex;

uniform uint Object;

void main(void)
{
	if (Object == 0){
		if (yAxis < 0){
			vec4 fieldTexColor = texture(grassTex, texCoordsExport);
			colorsOut = fieldTexColor * colorsExport;
		}
		else if(yAxis > 0 + 10){
			vec4 fieldTexColor = texture(waterTex, texCoordsExport);
			colorsOut = fieldTexColor;
		}
		else{
			float value = yAxis/10;
			vec4 waterTexColor = texture(waterTex, texCoordsExport);
			vec4 feildTexColor = texture(grassTex, texCoordsExport);
			colorsOut = (feildTexColor*(1-value))+(waterTexColor*(value));
		}
	}
	if (Object == 1){
		vec4 fieldTexColor = texture(waterTex, texCoordsExport);
		colorsOut = fieldTexColor;
	}
	if (Object == 2){
		colorsOut = colorsExport;
	}
}