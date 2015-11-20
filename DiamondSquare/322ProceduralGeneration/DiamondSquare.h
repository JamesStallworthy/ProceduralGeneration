#pragma once
#include <iostream>

class DiamondSquare {
private:
	void Diamond(int x1, int y1, int width, int iteration);
	void Square(int x1, int y1, int width, int iteration);
	float randomFloat(float min, float max);
public:
	DiamondSquare();
};