#include "stdafx.h"

#include <time.h>
#include <queue>
#include <unordered_map>

#include "Map.h"
#include "utils.cpp"



Cell& Map::getCell(int row, int col) {
	return cells[(row * maxRow) + col];
}

bool Map::inMap(int row, int col) {
	return row >= 0 && row <= maxRow
		&& col >= 0 && col <= maxCol;
}

struct LessThanByTotalCost
{
	bool operator()(const Cell& lhs, const Cell& rhs) const
	{
		return lhs.Type < rhs.Type();
	}
};

float heuristic(int destX, int destY, int neighborX, int neighborY) {
	return sqrdDistance(destX, destY, neighborX, neighborY);
}

void Map::getNeighbors(Cell& start, std::vector<Cell> neighbors) {
	neighbors[0] = getCell((start.Row() - 1), (start.Col() - 1));	// top left
	neighbors[1] = getCell((start.Row() - 1),  start.Col());		// top center
	neighbors[2] = getCell((start.Row() - 1), (start.Col() + 1));	// top right

	neighbors[3] = getCell(start.Row(),		  (start.Col() - 1));	// left
	neighbors[4] = getCell(start.Row(),		  (start.Col() + 1));	// right
	
	neighbors[5] = getCell((start.Row() + 1), (start.Col() - 1));	// bottom left
	neighbors[6] = getCell((start.Row() + 1), (start.Col() - 1));	// bottom center
	neighbors[7] = getCell((start.Row() + 1), (start.Col() - 1));	// bottom right
}


std::vector<uint32_t> Map::getPath(Cell& start, Cell& end) {
	std::priority_queue<Cell, std::vector<Cell>, LessThanByTotalCost> openCells;
	
	openCells.emplace(start);

	visitCost.emplace(start, 0, 0, INVALID_CELL_ID);

	while (!openCells.empty()) {
		auto current = openCells.top();
		openCells.pop();

		// Terrain == WALL
		if (current.Type() == IMPASSABLE)
			continue;

		// is this the destination?
		if (current.Id() == end.Id()) {
			// return reconstruct path function(current)
		}
		
		std::vector<Cell> neighbors(8);

		getNeighbors(current, neighbors);

		for (auto& neighbor : neighbors) {
			if (!inMap(neighbor.Row(), neighbor.Col()))
				continue;

			auto it = visitCost.find(neighbor.Id());
			int maybeTravelCost;
			
			// never seen this Cell before
			if (it == visitCost.end()) {
				// calculate travelCost if we decide to go here
				maybeTravelCost = neighbor.Type() + visitCost.at(current.Id()).travelCost;
				// add Cell to map
				visitCost.emplace(neighbor.Id());
			} 

			auto currentNeighborInfo = visitCost.at(neighbor.Id());

			// is the cost to go here from the current cell cheaper than what's recorded in the map?
			if (maybeTravelCost < currentNeighborInfo.travelCost) {
				// if so, add the better costs
				currentNeighborInfo.travelCost = maybeTravelCost;
				currentNeighborInfo.totalCost = maybeTravelCost + heuristic(end.Col(), end.Row(), neighbor.Col(), neighbor.Row());
				currentNeighborInfo.previousCell = current.Id();

				openCells.emplace(neighbor);
			}
		}
	}
}