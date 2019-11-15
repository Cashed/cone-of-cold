#pragma once

#include <vector>
#include <unordered_map>
#include "Cell.h"

struct PathCost {
	int travelCost = INT_MAX;
	uint32_t previousCell = 0;
};

struct Node
{
	uint32_t cellId = 0;
	float totalCost = std::numeric_limits<float>::infinity();
	bool operator<(const Node& rhs) const
	{
		return totalCost < rhs.totalCost;
	}
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


	int getCellIndex(int row, int col);
	Cell getCell(int row, int col);
	Cell getCellById(uint32_t id);
	bool inMap(int row, int col);
	std::vector<uint32_t> getPath(uint32_t start, uint32_t end);
	void getNeighbors(Cell& start, std::vector<Cell>& neighbors);
	
private:
	int maxRow = 20;
	int maxCol = 20;


	std::vector<Cell> cells;
};