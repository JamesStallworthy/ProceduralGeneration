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

uniform sampler2D sandTex;

uniform uint Object;

uniform float blendHeight;
uniform float blendHeightBottom;
uniform float blendHeightSnow;
uniform float blendDistance;

void main(void)
{
	if (Object == 0){
		//stone
		if(yAxis > blendHeight + blendDistance){
			vec4 fieldTexColor = texture(stoneTex, texCoordsExport);
			colorsOut = fieldTexColor * colorsExport;
		}
		if(yAxis< blendHeight && yAxis>blendHeightBottom){
			vec4 fieldTexColor = texture(grassTex, texCoordsExport);
			colorsOut = fieldTexColor * colorsExport;
		}
		if(yAxis < blendHeight + blendDistance && yAxis > blendHeight){
			float value = (yAxis-blendHeight)/blendDistance;
			vec4 stoneTexColor = texture(stoneTex, texCoordsExport);
			vec4 feildTexColor = texture(grassTex, texCoordsExport);
			colorsOut = ((feildTexColor*(1-value))+(stoneTexColor*(value)))*colorsExport;
		}
		if(yAxis < blendHeightBottom + blendDistance && yAxis > blendHeightBottom){
			float value = (yAxis-blendHeightBottom)/blendDistance;
			vec4 sandTexColor = texture(sandTex, texCoordsExport);
			vec4 feildTexColor = texture(grassTex, texCoordsExport);
			colorsOut = ((feildTexColor*(value))+(sandTexColor*(1-value)))*colorsExport;
		}
		if(yAxis < blendHeightBottom){
		vec4 fieldTexColor = texture(sandTex, texCoordsExport);
			colorsOut = fieldTexColor * colorsExport;
		}
		if(yAxis > blendHeightSnow){
			colorsOut = colorsExport;
		}

	}
	if (Object == 1){
		vec4 fieldTexColor = texture(waterTex, texCoordsExport);
		colorsOut = fieldTexColor;
	}
	if (Object == 2){
		colorsOut = colorsExport;
	}
	if (Object ==3 ){
		colorsOut = colorsExport;
	}
}

//	if (yAxis < blendHeight){
//			vec4 fieldTexColor = texture(grassTex, texCoordsExport);
//			colorsOut = fieldTexColor * colorsExport;
//		}
//		else if(yAxis > blendHeight + blendDistance){
//			vec4 fieldTexColor = texture(stoneTex, texCoordsExport);
//			colorsOut = fieldTexColor * colorsExport;
//		}
//		else{
//			float value = (yAxis-blendHeight)/blendDistance;
//			vec4 stoneTexColor = texture(stoneTex, texCoordsExport);
//			vec4 feildTexColor = texture(grassTex, texCoordsExport);
//			colorsOut = ((feildTexColor*(1-value))+(stoneTexColor*(value)))*colorsExport;
//		}