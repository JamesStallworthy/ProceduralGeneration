#include "tree.h"

void tree::fractalTree(int count, Vertex zero, Vertex one, float Angle)
{
	count--;
	if (count > 0) {
		vec2 Zero(zero.coords[0], zero.coords[1]);
		vec2 One(one.coords[0], one.coords[1]);

		vec2 Scale = (One - Zero);
		Scale = Scale * 0.8f;

		float alpha = (Angle * 3.14 / 180);
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

		fractalTree(count, branches[branchesIterator - 1].zero, branches[branchesIterator - 1].one, Angle);

		////////////////////////////////////////////////

		Zero = vec2(zero.coords[0], zero.coords[1]);
		One = vec2(one.coords[0], one.coords[1]);

		Scale = One - Zero;
		Scale = Scale * 0.8f;

		alpha = (Angle * 3.14 / 180)*-1;
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

		fractalTree(count, branches[branchesIterator - 1].zero, branches[branchesIterator - 1].one, Angle);
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

int tree::genTree(Vertex* drawVertices)
{
	drawVertices[0] = { { 0.0,-30.0f,0,1 } ,{ 0.3f,0.1f,0,1 } };
	drawVertices[1] = { { 0.0, -15.0f, 0,1 },{ 0.3f,0.1f,0,1 } };

	fractalTree(5, drawVertices[0], drawVertices[1], 70);
	cout << branchesIterator << endl;

	int q = 0;
	int leafIterator = 0;
	int end_of_branches = 0;
	for (int i = 2; i <= branchesIterator*2; i += 2) {
		drawVertices[i] = branches[q].zero;
		drawVertices[i + 1] = branches[q].one;
		if (branches[q].end) {
			end_of_branches++;
			leaves[leafIterator].base = branches[q].one;
			leafIterator++;
		}
		q++;
	}

	//GenLeaves
	for (int i = 0; i < 16; i++) {
		genLeaves(i);
	}
	//Add leaves to buffer
	int z = 0;
	cout << branchesIterator * 2 + 2 << endl;
	for (int i = branchesIterator * 2 +2; i <  (branchesIterator * 2+2) + 48; i += 3) {
		drawVertices[i] = leaves[z].base;
		drawVertices[i + 1] = leaves[z].left;
		drawVertices[i + 2] = leaves[z].right;
		z++;
	}
	return 0;
}
