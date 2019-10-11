#include "stdafx.h"

#include <time.h>
#include <queue>

#include "Map.h"
#include "utils.cpp"

Map::Map(int _maxRow, int _maxCol) {
	auto size = _maxRow * _maxCol;

	srand(time(NULL));

	cells.resize = size;
	
	for (auto i = 0; i < size; ++i) {
		cells.emplace_back(rand() % MAX + 1);
	}
}

Map::Map() {
	Map(maxRow, maxCol);
}

Cell& Map::getCell(int row, int col) {
	return cells[(row * maxRow) + col];
}

struct LessThanByTerrain
{
	bool operator()(const Cell& lhs, const Cell& rhs) const
	{
		return lhs.Type() < rhs.Type();
	}
};

float heuristic(int currentX, int currentY, int neighborX, int neighborY) {
	return sqrdDistance(currentX, currentY, neighborX, neighborY);
}

void getNeighbors(Cell& start, std::vector<Cell>& closedCells, std::vector<Cell>& neighbors) {

}

void getPath(Cell& start, Cell& end) { 

	std::priority_queue<Cell, std::vector<Cell>, LessThanByTerrain> openCells;
	std::vector<Cell> closedCells;
	
	openCells.emplace(start);

	while (!openCells.empty()) {
		auto current = openCells.top();
		openCells.pop();

		// is this the destination?
		if (current.Id() == end.Id()) {
			// return reconstruct path function
		}

		closedCells.emplace_back(current);
		
		std::vector<Cell> neighbors(8);

		getNeighbors(current, closedCells, neighbors);

		for (auto& neighbor : neighbors) {
			// 
		}
	}
}