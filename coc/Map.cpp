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

bool Map::inMap(int row, int col) {
	return row >= 0 && row <= maxRow
		&& col >= 0 && col <= maxCol;
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

void getNeighbors(Cell& start, std::vector<Cell> neighbors, Map& map) {
	neighbors[0] = map.getCell((start.Row() - 1), (start.Col() - 1));	// top left
	neighbors[1] = map.getCell((start.Row() - 1),  start.Col());		// top center
	neighbors[2] = map.getCell((start.Row() - 1), (start.Col() + 1));	// top right

	neighbors[3] = map.getCell(start.Row(),		  (start.Col() - 1));	// left
	neighbors[4] = map.getCell(start.Row(),		  (start.Col() + 1));	// right
	
	neighbors[5] = map.getCell((start.Row() + 1), (start.Col() - 1));	// bottom left
	neighbors[6] = map.getCell((start.Row() + 1), (start.Col() - 1));	// bottom center
	neighbors[7] = map.getCell((start.Row() + 1), (start.Col() - 1));	// bottom right
}

void getPath(Cell& start, Cell& end, Map& map) { 
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

		getNeighbors(current, neighbors, map);

		for (auto& neighbor : neighbors) {
			if (!map.inMap(neighbor.Row(), neighbor.Col()))
				continue;

			if (neighbor.Type() == IMPASSABLE)
				continue;

			auto it = visitCost.find(neighbor.Id());
			
			int cost;
			if (it == visitCost.end()) {
				cost = neighbor.Type() + visitCost.at(current.Id()).cost;
			} else {
				cost = it->second.cost;
			}

			if (cost) {
				visitCost.emplace(neighbor.Id(), cost, current.Id());
				heuristic(end.Col(), end.Row(), neighbor.Col(), neighbor.Row());
			}
		}
	}
}