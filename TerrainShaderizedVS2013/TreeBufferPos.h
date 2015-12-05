#pragma once
#include <vector>

struct TreeBufferPos {
	int treeStart;
	int treeFinish;
	int leafStart;
	int leafFinish;
	std::vector<int> depth;
};
