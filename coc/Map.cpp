#include "stdafx.h"

#include <time.h>
#include <queue>
#include <unordered_map>

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

float heuristic(int destX, int destY, int neighborX, int neighborY) {
	return sqrdDistance(destX, destY, neighborX, neighborY);
}

void getNeighbors(Cell& start, std::vector<Cell>& closedCells, std::vector<Cell>& neighbors) {

}

void getPath(Cell& start, Cell& end) { 

	std::priority_queue<Cell, std::vector<Cell>, LessThanByTerrain> openCells;
	
	openCells.emplace(start);

	// unordered map, key = cell id, val = cost
	std::unordered_map<_int32, PathCost> visitCost;
	visitCost.emplace(start.Id(), 0, INVALID_CELL_ID);

	while (!openCells.empty()) {
		auto current = openCells.top();
		openCells.pop();

		// is this the destination?
		if (current.Id() == end.Id()) {
			// return reconstruct path function(current)
		}
		
		std::vector<Cell> neighbors(8);

		getNeighbors(current, closedCells, neighbors);

		for (auto& neighbor : neighbors) {
			int neighborCost;
			auto it = visitCost.find(neighbor.Id());
			if (it == visitCost.end()) {
			}
			else {
				neighborCost = visitCost[neighbor.Id()];
			}


		}
	}
}