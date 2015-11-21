#include "tree.h"

void tree::fractalTree(int count, Vertex zero, Vertex one, float Angle, float rand)
{
	count--;
	if (count > 0) {
		vec2 Zero(zero.coords[0], zero.coords[1]);
		vec2 One(one.coords[0], one.coords[1]);

		vec2 Scale = (One - Zero);
		Scale = Scale * 0.8f;

		float alpha = ((Angle + randomFloat(-rand, rand))*3.14 / 180);
		mat2x2 rotMatrix(cos(alpha / 2), sin(alpha / 2), -sin(alpha / 2), cos(alpha / 2));
		vec2 Answer = rotMatrix * Scale;

		Answer = Answer + One;

		branches[branchesIterator].zero = one;
		branches[branchesIterator].zero.colors[0] = 0.3f;
		branches[branchesIterator].zero.colors[1] = 0.1f;
		branches[branchesIterator].one.coords[0] = Answer.x;
		branches[branchesIterator].one.coords[1] = Answer.y;
		branches[branchesIterator].one.coords[2] = 0;
		branches[branchesIterator].one.coords[3] = 1;
		branches[branchesIterator].one.coords[3] = 1;
		branches[branchesIterator].one.colors[0] = 0.3f;
		branches[branchesIterator].one.colors[1] = 0.1f;
		if (count == 1)
			branches[branchesIterator].end = true;
		else
			branches[branchesIterator].end = false;
		branchesIterator++;

		fractalTree(count, branches[branchesIterator - 1].zero, branches[branchesIterator - 1].one, Angle, rand);

		////////////////////////////////////////////////

		Zero = vec2(zero.coords[0], zero.coords[1]);
		One = vec2(one.coords[0], one.coords[1]);

		Scale = One - Zero;
		Scale = Scale * 0.8f;
		alpha = ((Angle + randomFloat(-rand, rand))*3.14 / 180)*-1;
		rotMatrix = mat2x2(cos(alpha / 2), sin(alpha / 2), -sin(alpha / 2), cos(alpha / 2));
		Answer = rotMatrix * Scale;

		Answer = Answer + One;
		branches[branchesIterator].zero = one;
		branches[branchesIterator].zero.colors[0] = 0.3f;
		branches[branchesIterator].zero.colors[1] = 0.1f;
		branches[branchesIterator].one.coords[0] = Answer.x;
		branches[branchesIterator].one.coords[1] = Answer.y;
		branches[branchesIterator].one.coords[2] = 0;
		branches[branchesIterator].one.coords[3] = 1;
		branches[branchesIterator].one.coords[3] = 1;
		branches[branchesIterator].one.colors[0] = 0.3f;
		branches[branchesIterator].one.colors[1] = 0.1f;
		if (count == 1)
			branches[branchesIterator].end = true;
		else
			branches[branchesIterator].end = false;
		branchesIterator++;

		fractalTree(count, branches[branchesIterator - 1].zero, branches[branchesIterator - 1].one, Angle,rand);
	}
}
void tree::genLeaves(int leafID) {
	//Colour
	leaves[leafID].base.colors[1] = 1.0f;
	leaves[leafID].left.colors[1] = 1.0f;
	leaves[leafID].right.colors[1] = 1.0f;
	//Coords
	leaves[leafID].left.coords[0] = leaves[leafID].base.coords[0] - 2;
	leaves[leafID].left.coords[1] = leaves[leafID].base.coords[1] + 2;
	leaves[leafID].left.coords[2] = 0;
	leaves[leafID].right.coords[0] = leaves[leafID].base.coords[0] + 2;
	leaves[leafID].right.coords[1] = leaves[leafID].base.coords[1] + 2;
	leaves[leafID].right.coords[2] = 0;
	//4th value in coord needs to be 1
	leaves[leafID].base.coords[3] = 1;
	leaves[leafID].left.coords[3] = 1;
	leaves[leafID].right.coords[3] = 1;
}

float tree::randomFloat(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random*range) + min;
}

TreeBufferPos tree::genTree(Vertex* drawVertices, int startPoint, int count, float angle)
{
	drawVertices[startPoint] = { { 0.0,-30.0f,0,1 } ,{ 0.3f,0.1f,0,1 } };
	drawVertices[startPoint+1] = { { 0.0, -15.0f, 0,1 },{ 0.3f,0.1f,0,1 } };

	fractalTree(count, drawVertices[startPoint], drawVertices[startPoint+1], angle, 30);
	cout << "Number of branches: "<<branchesIterator << endl;

	int q = 0;
	int leafIterator = 0;
	for (int i = startPoint+2; i <= startPoint + branchesIterator*2 +1; i += 2) {
		drawVertices[i] = branches[q].zero;
		drawVertices[i + 1] = branches[q].one;
		if (branches[q].end) {
			leaves[leafIterator].base = branches[q].one;
			leafIterator++;
		}
		q++;
	}
	cout << "Number of leaves " << leafIterator << endl;
	//GenLeaves
	//Somereason always one extra leaf;
	for (int i = 0; i < leafIterator; i++) {
		genLeaves(i);
	}
	//Add leaves to buffer
	int z = 0;
	for (int i = startPoint + branchesIterator * 2 +2; i < startPoint + (branchesIterator * 2+2) + (leafIterator)*3; i += 3) {
		drawVertices[i] = leaves[z].base;
		drawVertices[i + 1] = leaves[z].left;
		drawVertices[i + 2] = leaves[z].right;
		z++;
	}
	cout << "Tree starts at buffer: " << startPoint << endl;
	cout << "Tree ends at buffer: " << startPoint+branchesIterator * 2 + 1 << endl;
	cout << "Leaves start at buffer: " << startPoint+branchesIterator * 2 + 2 << endl;
	cout << "Leaves ends at buffer: " << startPoint+((branchesIterator * 2 + 2) + (leafIterator)* 3) << endl;
	TreeBufferPos pos;
	pos.treeStart = startPoint;
	pos.treeFinish = startPoint + branchesIterator * 2 + 1;
	pos.leafStart = startPoint + branchesIterator * 2 + 2;
	pos.leafFinish = startPoint + ((branchesIterator * 2 + 2) + (leafIterator)* 3);
	return pos;
}
