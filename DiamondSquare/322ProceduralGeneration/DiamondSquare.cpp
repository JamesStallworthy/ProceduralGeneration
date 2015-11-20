#include "DiamondSquare.h"
/*
float DiamondSquare::randomFloat(float min, float max) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random*range) + min;
}

void DiamondSquare::Diamond(int x1, int y1, int width, int iteration) {
	//Diamond
	terrain[x1 + (width) / 2][y1 + (width) / 2] = ((terrain[x1][y1] + terrain[x1 + width][y1] + terrain[x1][y1 + width] + terrain[x1 + width][y1 + width]) / 4) + randomFloat(-RAND_AMOUNT, RAND_AMOUNT) / iteration;
}

void DiamondSquare::Square(int x1, int y1, int width, int iteration) {
	//Sqaure
	//top
	if (y1 - width / 2 < 0)
		terrain[x1 + width / 2][y1] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1] + terrain[x1 + width][y1]) / 3) + randomFloat(-RAND_AMOUNT, RAND_AMOUNT) / iteration;
	else
		terrain[x1 + width / 2][y1] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1] + terrain[x1 + width][y1] + terrain[x1 + width / 2][y1 - width / 2]) / 4) + randomFloat(-RAND_AMOUNT, RAND_AMOUNT) / iteration;
	//Left
	if (x1 - width / 2 < 0)
		terrain[x1][y1 + width / 2] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1] + terrain[x1][y1 + width]) / 3) + randomFloat(-RAND_AMOUNT, RAND_AMOUNT) / iteration;
	else
		terrain[x1][y1 + width / 2] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1] + terrain[x1][y1 + width] + terrain[x1 - width / 2][y1 + width / 2]) / 4) + randomFloat(-RAND_AMOUNT, RAND_AMOUNT) / iteration;
	//Right
	if (x1 + width + width / 2 > MAP_SIZE - 1)
		terrain[x1 + width][y1 + width / 2] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1 + width][y1] + terrain[x1 + width][y1 + width]) / 3) + randomFloat(-RAND_AMOUNT, RAND_AMOUNT) / iteration;
	else
		terrain[x1 + width][y1 + width / 2] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1 + width][y1] + terrain[x1 + width][y1 + width] + terrain[x1 + width + width / 2][y1 + width / 2]) / 4) + randomFloat(-RAND_AMOUNT, RAND_AMOUNT) / iteration;
	//Bottom
	if (y1 + width + width / 2 > MAP_SIZE)
		terrain[x1 + width / 2][y1 + width] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1 + width] + terrain[x1 + width][y1 + width]) / 3) + randomFloat(-RAND_AMOUNT, RAND_AMOUNT) / iteration;
	else
		terrain[x1 + width / 2][y1 + width] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1 + width] + terrain[x1 + width][y1 + width] + terrain[x1 + width / 2][y1 + width + width / 2]) / 4) + randomFloat(-RAND_AMOUNT, RAND_AMOUNT) / iteration;
}
*/