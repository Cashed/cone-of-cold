#pragma once

#include <vector>
#include <unordered_map>
#include "Cell.h"

struct PathCost {
	float totalCost = INT_MAX;
	int travelCost = INT_MAX;
	uint32_t previousCell = 0;
};

class Map {
public:
	Map() { Map(maxRow, maxCol); };

	Map(int _maxRow, int _maxCol) : maxRow(_maxRow), maxCol(_maxCol) {
		auto size = _maxRow * _maxCol;

		srand(time(NULL));

		cells.resize(size);

		for (auto i = 0; i < size; ++i) {
			cells.emplace_back(rand() % MAX + 1);
		}
	};


	Cell& getCell(int row, int col);
	bool inMap(int row, int col);
	std::vector<uint32_t> getPath(Cell& start, Cell& end);;
	void getNeighbors(Cell& start, std::vector<Cell> neighbors);
	
private:
	int maxRow = 20;
	int maxCol = 20;

	// unordered map, key = cell id, val = PathCost { travelCost, totalCost, previousCell }
	std::unordered_map<_int32, PathCost> visitCost;

	std::vector<Cell> cells;
};