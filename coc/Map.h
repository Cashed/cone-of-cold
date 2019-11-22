#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Cell.h"

struct PathCost {
	int travelCost = INT_MAX;
	uint32_t previousCell = INVALID_CELL_ID;
};

struct Node
{
	Node(uint32_t _cellId, float _totalCost): cellId(_cellId), totalCost(_totalCost) {};
	uint32_t cellId = 0;
	float totalCost = std::numeric_limits<float>::infinity();
	bool operator<(const Node& rhs) const
	{
		return totalCost < rhs.totalCost;
	}
};


class Map {
public:
	Map() { Map(20, 20); };
	Map(int _maxRow, int _maxCol) : maxRow(_maxRow), maxCol(_maxCol) {
		auto size = _maxRow * _maxCol;

		srand(static_cast<unsigned int>(time(NULL)));

		cells.reserve(size);

		for (auto i = 0; i < size; ++i) {
			cells.emplace_back(static_cast<Terrain>(rand() % MAX + 1), i / maxRow, i % maxRow);
		}

		std::cout << "created " << cells.size() << " cells\n";
		std::cout << "the first is " << cells[0].Type() << "\n";
	};


	int getCellIndex(int row, int col);
	Cell& getCell(int row, int col);
	Cell& getCellById(uint32_t id);
	bool inMap(int row, int col);
	std::vector<uint32_t> getPath(uint32_t start, uint32_t end);
	void getNeighbors(Cell& start, std::vector<Cell>& neighbors);
	
private:
	int maxRow = 20;
	int maxCol = 20;


	std::vector<Cell> cells;
};