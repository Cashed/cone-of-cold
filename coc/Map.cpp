#include "stdafx.h"

#include <time.h>
#include "Map.h"

Map::Map(int _maxRow, int _maxCol) {
	auto size = _maxRow * _maxCol;

	srand(time(NULL));

	for (auto i = 0; i < size; ++i) {
		cells.emplace_back(Cell(Terrain(rand() % MAX + 1)));
	}
}

Map::Map() {
	Map(maxRow, maxCol);
}

Cell& Map::getCell(int row, int col) {

}