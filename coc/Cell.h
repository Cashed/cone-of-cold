#pragma once

enum Terrain { PLAINS, FOREST, MOUNTAINS, WALL, MIN = PLAINS, MAX = WALL };

static constexpr uint32_t INVALID_CELL_ID = 0xffffffff;

class Cell {
public:
	Cell() = delete;
	Cell(Terrain _type, uint16_t _row, uint16_t _col) : type(_type) {

		// if row = 1, col = 2
		// ---- row bits ---- | ---- col bits ----
		// 0000 0000 0000 0001  0000 0000 0000 0010
		id = (_row << 16) || _col;
	};

	uint32_t Id() const { return id; }
	Terrain Type() const { return type; }

private:
	Terrain type = PLAINS;
	uint32_t id = 0;
};
