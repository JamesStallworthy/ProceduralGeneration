#include "tree.h"
#include <time.h>

void tree::fractalTree(int count, Vertex zero, Vertex one, float angleRight, float angleLeft, float rand, float randBack)
{
	count--;
	if (count > 0) {
		vec4 Zero(zero.coords[0], zero.coords[1],zero.coords[2],1);
		vec4 One(one.coords[0], one.coords[1], one.coords[2],1);

		vec4 Scale = (One - Zero);
		Scale = Scale * 0.8f;
		//float randValue = randomFloat(-rand, rand);
		//float alpha = ((angleRight + randValue)*3.14 / 180);
		////mat3x3 rotMatrix(cos(alpha), sin(alpha), -sin(alpha), cos(alpha));
		//mat3x3 rotMatrixX(1, 0, 0, 0, cos(alpha), sin(alpha), 0, -sin(alpha), cos(alpha));
		//float randValue2 = randomFloat(-rand, rand);
		//float beta = ((angleRight + randValue2)*3.14 / 180);
		//mat3x3 rotMatrixZ(cos(beta), sin(beta), 0, -sin(beta), cos(beta), 0, 0, 0, 1);
		//vec3 Answer = (Scale * rotMatrixZ)*rotMatrixX;
		mat4 rotMatrix = mat4(1);
		rotMatrix = rotate(rotMatrix, randomFloat((-angleRight)+rand, angleRight+rand)*3.14f/180.0f, vec3(0, 1, 0));
		rotMatrix = rotate(rotMatrix, randomFloat((-angleRight) + rand, angleRight + rand)*3.14f / 180.0f, vec3(0, 0, 1));
		rotMatrix = rotate(rotMatrix, randomFloat((-angleRight) + rand, angleRight + rand)*3.14f / 180.0f, vec3(1, 0, 0));
		vec4 Answer = rotMatrix * Scale;
		Answer = Answer + One;

		branches[branchesIterator].zero = one;
		branches[branchesIterator].zero.colors[0] = 0.3f;
		branches[branchesIterator].zero.colors[1] = 0.1f;
		branches[branchesIterator].one.coords[0] = Answer.x;
		branches[branchesIterator].one.coords[1] = Answer.y;
		branches[branchesIterator].one.coords[2] = Answer.z;
		branches[branchesIterator].one.coords[3] = 1;
		branches[branchesIterator].one.coords[3] = 1;
		branches[branchesIterator].one.colors[0] = 0.3f;
		branches[branchesIterator].one.colors[1] = 0.1f;
		if (count == 1)
			branches[branchesIterator].end = true;
		else
			branches[branchesIterator].end = false;
		branchesIterator++;
		depth.push_back(count);
		fractalTree(count, branches[branchesIterator - 1].zero, branches[branchesIterator - 1].one, angleRight, angleLeft, rand,randBack);

		////////////////////////////////////////////////

		Zero = vec4(zero.coords[0], zero.coords[1], zero.coords[2],1);
		One = vec4(one.coords[0], one.coords[1], one.coords[2],1);

		Scale = One - Zero;
		Scale = Scale * 0.8f;
		/*randValue = randomFloat(-rand, rand);
		alpha = ((angleLeft +randValue)*3.14 / 180)*-1;
		rotMatrixX = mat3x3(1, 0, 0, 0, cos(alpha), sin(alpha), 0, -sin(alpha), cos(alpha));
		randValue2 = randomFloat(-rand, rand);
		beta = ((angleLeft + randValue2)*3.14 / 180)*-1;
		rotMatrixZ = mat3x3(cos(beta), sin(beta), 0, -sin(beta), cos(beta), 0, 0, 0, 1);
		Answer = (Scale * rotMatrixZ)*rotMatrixX;*/

		rotMatrix = mat4(1);
		rotMatrix = rotate(rotMatrix, (randomFloat((-angleLeft)+rand, angleLeft+rand)*3.14f/180.0f), vec3(0, 1, 0));
		rotMatrix = rotate(rotMatrix, randomFloat((-angleLeft) + rand, angleLeft + rand)*3.14f / 180.0f, vec3(0, 0, 1));
		rotMatrix = rotate(rotMatrix, randomFloat((-angleLeft) + rand, angleLeft + rand)*3.14f / 180.0f, vec3(1, 0, 0));

		Answer = rotMatrix * Scale;
		Answer = Answer + One;

		branches[branchesIterator].zero = one;
		branches[branchesIterator].zero.colors[0] = 0.3f;
		branches[branchesIterator].zero.colors[1] = 0.1f;
		branches[branchesIterator].one.coords[0] = Answer.x;
		branches[branchesIterator].one.coords[1] = Answer.y;
		branches[branchesIterator].one.coords[2] = Answer.z;
		branches[branchesIterator].one.coords[3] = 1;
		branches[branchesIterator].one.coords[3] = 1;
		branches[branchesIterator].one.colors[0] = 0.3f;
		branches[branchesIterator].one.colors[1] = 0.1f;
		if (count == 1)
			branches[branchesIterator].end = true;
		else
			branches[branchesIterator].end = false;
		branchesIterator++;
		depth.push_back(count);
		fractalTree(count, branches[branchesIterator - 1].zero, branches[branchesIterator - 1].one, angleRight,angleLeft,rand,randBack);
	}
}
void tree::genLeaves(int leafID) {
	//Colour
	leaves[leafID].base.colors[1] = 0.5f;
	leaves[leafID].left.colors[1] = 0.6f;
	leaves[leafID].right.colors[1] = 0.6f;
	leaves[leafID].top.colors[1] = 1.0f;
	//Coords
	vec4 left(- 4,  4,0,0);
	vec4 right(4,4,0,0);
	vec4 top(0, 10,0,0);

	//Rotate
	float alpha = randomFloat(0,360);
	//mat3 rotMatrix(cos(alpha), sin(alpha),0.0f, -sin(alpha), cos(alpha),0.0f,0.0f,0.0f,1.0f);
	mat4 rotMatrix = mat4(1);
	rotMatrix = rotate(mat4(1),alpha, vec3(0,0,1));
	rotMatrix = rotate(rotMatrix, alpha, vec3(0, 1, 0));
	rotMatrix = rotate(rotMatrix, alpha, vec3(1, 0, 0));
	left = rotMatrix * left;
	right = rotMatrix * right;
	top =  rotMatrix * top;

	leaves[leafID].left.coords[0] = leaves[leafID].base.coords[0] + left.x;
	leaves[leafID].left.coords[1] = leaves[leafID].base.coords[1] + left.y;
	leaves[leafID].left.coords[2] = leaves[leafID].base.coords[2] + left.z;
	leaves[leafID].right.coords[0] = leaves[leafID].base.coords[0] + right.x;
	leaves[leafID].right.coords[1] = leaves[leafID].base.coords[1] + right.y;
	leaves[leafID].right.coords[2] = leaves[leafID].base.coords[2] + right.z;
	leaves[leafID].top.coords[0] = leaves[leafID].base.coords[0] + top.x;
	leaves[leafID].top.coords[1] = leaves[leafID].base.coords[1] + top.y;
	leaves[leafID].top.coords[2] = leaves[leafID].base.coords[2] + top.z;
	//4th value in coord needs to be 1
	leaves[leafID].base.coords[3] = 1;
	leaves[leafID].left.coords[3] = 1;
	leaves[leafID].right.coords[3] = 1;
	leaves[leafID].top.coords[3] = 1;
}

float tree::randomFloat(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random*range) + min;
}

TreeBufferPos tree::genTree(Vertex* drawVertices, int startPoint, int count, float angleRight, float angleLeft, float height)
{
	drawVertices[startPoint] = { { 0.0,0.0f,0,1 } ,{0,0,0} , {0,0},{ 0.3f,0.1f,0,1 } };
	drawVertices[startPoint+1] = { { 0.0, height, 0,1 },{ 0,0,0 } ,{ 0,0 },{ 0.3f,0.1f,0,1 } };
	srand(time(0));
	fractalTree(count, drawVertices[startPoint], drawVertices[startPoint+1], angleRight, angleLeft, 0,0);
	
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
	for (int i = startPoint + branchesIterator * 2 +2; i < startPoint + (branchesIterator * 2+2) + (leafIterator)*4; i += 4) {
		drawVertices[i] = leaves[z].base;
		drawVertices[i + 1] = leaves[z].left;
		drawVertices[i + 2] = leaves[z].right;
		drawVertices[i + 3] = leaves[z].top;
		z++;
	}
	cout << "Tree starts at buffer: " << startPoint << endl;
	cout << "Tree ends at buffer: " << startPoint+branchesIterator * 2 + 1 << endl;
	cout << "Leaves start at buffer: " << startPoint+branchesIterator * 2 + 2 << endl;
	cout << "Leaves ends at buffer: " << startPoint+((branchesIterator * 2 + 2) + (leafIterator)* 4) << endl;
	TreeBufferPos pos;
	pos.treeStart = startPoint;
	pos.treeFinish = startPoint + branchesIterator * 2 + 1;
	pos.leafStart = startPoint + branchesIterator * 2 + 2;
	pos.leafFinish = startPoint + ((branchesIterator * 2 + 2) + (leafIterator)* 4);
	pos.depth = depth;
	return pos;
}
