#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.h"
#include "Branch.h"
#include "Leaf.h"

using namespace std;
using namespace glm;


class tree {
private:
	Branch branches[1000];
	Leaf leaves[1000];
	int branchesIterator;
	void fractalTree(int count, Vertex zero, Vertex one, float Angle);
	void genLeaves(int i);
public:
	tree() { branchesIterator = 0; };
	int genTree(Vertex* drawVertices);
};