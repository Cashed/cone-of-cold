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
		return lhs.Type() < rhs.Type();
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


std::vector<uint32_t> Map::getPath(Cell& start, Cell& end) 
{
	struct Node 
	{
		uint32_t cellId = 0;
		float totalCost = 4000; // TODO: make this very big
		bool operator<(const Node& rhs) const
		{
			return totalCost < rhs.totalCost;
		}
 	};
	
	std::priority_queue<Node> openCells;

	// unordered map, key = cell id, val = PathCost { travelCost, totalCost, previousCell }
	std::unordered_map<uint32_t, PathCost> visitCost;
	
	openCells.emplace(start.Id(), 0);

	visitCost.emplace(start.Id(), 0, 0, INVALID_CELL_ID);

	while (!openCells.empty()) {
		auto currentNode = openCells.top();
		openCells.pop();

		const Cell& currentCell = GetCellById(currentNode.cellId); // TODO: make this

		// is this the destination?
		if (currentCell.Id() == end.Id()) {
			// return reconstruct path function(current)
		}
		
		std::vector<Cell> neighbors(8);

		getNeighbors(currentCell, neighbors);

		for (auto& neighbor : neighbors) {
			if (!inMap(neighbor.Row(), neighbor.Col()))
				continue;

			// Terrain == WALL
			if (neighbor.Type() == IMPASSABLE)
				continue;
			
			// calculate travelCost if we decide to go here
			auto neighborCost = neighbor.Type();
			int maybeTravelCost = neighborCost + visitCost.at(current.Id()).travelCost;

			auto currentNeighborInfo = visitCost[neighbor.Id()];

			// is the cost to go here from the current cell cheaper than what's recorded in the map?
			if (maybeTravelCost < currentNeighborInfo.travelCost) {
				// if so, add the better costs
				currentNeighborInfo.travelCost = maybeTravelCost;
				auto totalCost = maybeTravelCost + heuristic(end.Col(), end.Row(), neighbor.Col(), neighbor.Row());
				currentNeighborInfo.previousCell = current.Id();

				openCells.emplace(neighbor.Id(), totalCost);
			}
		}
	}
}