#version 430 core

#define SQUARE_VERTICES 0
#define SKY_VERTICES 1

smooth in vec4 colorsExport;

out vec4 colorsOut;

in vec2 texCoordsExport;

smooth in float yAxis;

uniform sampler2D grassTex;

uniform sampler2D waterTex;

uniform sampler2D stoneTex;

uniform uint Object;

uniform float blendHeight;
uniform float blendDistance;

void main(void)
{
	if (Object == 0){
		if (yAxis < blendHeight){
			vec4 fieldTexColor = texture(grassTex, texCoordsExport);
			colorsOut = fieldTexColor * colorsExport;
		}
		else if(yAxis > blendHeight + blendDistance){
			vec4 fieldTexColor = texture(stoneTex, texCoordsExport);
			colorsOut = fieldTexColor * colorsExport;
		}
		else{
			float value = (yAxis-blendHeight)/blendDistance;
			vec4 stoneTexColor = texture(stoneTex, texCoordsExport);
			vec4 feildTexColor = texture(grassTex, texCoordsExport);
			colorsOut = ((feildTexColor*(1-value))+(stoneTexColor*(value)))*colorsExport;
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