#pragma once

#include <vector>
#include "Cell.h"

struct PathCost {
	int cost = INT_MAX;
	uint32_t previousCell = 0;
};

class Map {
public:
	Map() {};
	Map(int _maxRow, int _maxCol) : maxRow(_maxRow), maxCol(_maxCol) {};

	Cell& getCell(int row, int col);
	bool inMap(int row, int col);
	
private:
	int maxRow = 20;
	int maxCol = 20;

	std::vector<Cell> cells;
};