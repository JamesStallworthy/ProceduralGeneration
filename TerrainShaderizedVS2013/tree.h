#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.h"
#include "Branch.h"
#include "Leaf.h"
#include "TreeBufferPos.h"
#include <vector>

using namespace std;
using namespace glm;


class tree {
private:
	Branch branches[2000];
	Leaf leaves[2000];
	int branchesIterator;
	void fractalTree(int count, Vertex zero, Vertex one, float angleRight, float angleLeft, float rand, float randBack);
	void genLeaves(int i);
	float randomFloat(float min, float max);
	vector <int> depth;
public:
	tree() { branchesIterator = 0; };
	TreeBufferPos genTree(Vertex* drawVertices, int startpoint, int count, float angleRight, float angleLeft, float height);
};