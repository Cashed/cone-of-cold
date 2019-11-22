#include "stdafx.h"

#include <time.h>
#include <queue>
#include <unordered_map>
#include <iostream>

#include "Map.h"
#include "MathUtils.h"


Cell& Map::getCell(int row, int col) {
	return cells[(row * maxRow) + col];
}

int Map::getCellIndex(int row, int col) {
	auto index = (row * maxRow) + col;
	if (index < 0 || index >= (maxRow * maxCol)) {
		return -1;
	}
	return index;
}

bool Map::inMap(int row, int col) {
	return row >= 0 && row <= maxRow
		&& col >= 0 && col <= maxCol;
}


float heuristic(int destX, int destY, int neighborX, int neighborY) {
	return sqrdDistance(destX, destY, neighborX, neighborY);
}

void Map::getNeighbors(Cell& start, std::vector<Cell>& neighbors) {
	auto topLeft = getCellIndex((start.Row() - 1), (start.Col() - 1));
	if (topLeft > -1) {
		neighbors.emplace_back(cells[topLeft]);
	}

	auto topCenter = getCellIndex((start.Row() - 1), start.Col());
	if (topCenter > -1) {
		neighbors.emplace_back(cells[topCenter]);
	}

	auto topRight = getCellIndex((start.Row() - 1), (start.Col() + 1));
	if (topRight > -1) {
		neighbors.emplace_back(cells[topRight]);
	}

	auto left = getCellIndex(start.Row(), (start.Col() - 1));
	if (left > -1) {
		neighbors.emplace_back(cells[left]);
	}

	auto right = getCellIndex(start.Row(), (start.Col() + 1));
	if (right > -1) {
		neighbors.emplace_back(cells[right]);
	}

	auto bottomLeft = getCellIndex((start.Row() + 1), (start.Col() - 1));
	if (bottomLeft > -1) {
		neighbors.emplace_back(cells[bottomLeft]);
	}

	auto bottomCenter = getCellIndex((start.Row() + 1), (start.Col() - 1));
	if (bottomCenter > -1) {
		neighbors.emplace_back(cells[bottomCenter]);
	}

	auto bottomRight = getCellIndex((start.Row() + 1), (start.Col() - 1));
	if (bottomRight > -1) {
		neighbors.emplace_back(cells[bottomRight]);
	}
}

Cell& Map::getCellById(uint32_t id) {
	for (auto& cell : cells) {
		if (cell.Id() == id) {
			return cell;
		}
	}
}

std::vector <uint32_t> pathToDestination(uint32_t currentId, std::unordered_map<uint32_t, PathCost>& visitCost) {
	auto path = std::vector<uint32_t>{};
	path.emplace_back(currentId);

	auto current = visitCost[currentId].previousCell;
	
	while (current != INVALID_CELL_ID) {
		path.emplace_back(current);
		current = visitCost[current].previousCell;
	}

	return path;
}


std::vector<uint32_t> Map::getPath(uint32_t start, uint32_t end) 
{	
	std::priority_queue<Node> openCells;

	// unordered map, key = cell id, val = PathCost { travelCost,  previousCell }
	std::unordered_map<uint32_t, PathCost> visitCost;

	auto endCell = getCellById(end);
	
	openCells.emplace(start, 0);

	auto it = visitCost[start];


	while (!openCells.empty()) {
		auto currentNode = openCells.top();
		openCells.pop();

		auto currentCell = getCellById(currentNode.cellId);

		// is this the destination?
		if (currentCell.Id() == end) {
			std::cout << "found the end!\n";
			return pathToDestination(currentCell.Id(), visitCost);
		}
		
		std::vector<Cell> neighbors;

		getNeighbors(currentCell, neighbors);

		for (auto& neighbor : neighbors) {
			if (!inMap(neighbor.Row(), neighbor.Col()))
				continue;

			// Terrain == WALL
			if (neighbor.Type() == IMPASSABLE)
				continue;
			
			// calculate travelCost if we decide to go here
			auto neighborCost = neighbor.Type();
			int maybeTravelCost = neighborCost + visitCost.at(currentCell.Id()).travelCost;

			auto currentNeighborInfo = visitCost[neighbor.Id()];

			// is the cost to go here from the current cell cheaper than what's recorded in the map?
			if (maybeTravelCost < currentNeighborInfo.travelCost) {
				// if so, add the better costs
				currentNeighborInfo.travelCost = maybeTravelCost;
				auto totalCost = maybeTravelCost + heuristic(endCell.Col(), endCell.Row(), neighbor.Col(), neighbor.Row());
				currentNeighborInfo.previousCell = currentCell.Id();

				openCells.emplace(neighbor.Id(), totalCost);
			}
		}
	}
}