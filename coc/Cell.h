#pragma once

enum Terrain { PLAINS, FOREST, MOUNTAINS, WALL, MIN = PLAINS, MAX = WALL };

static constexpr uint32_t INVALID_CELL_ID = 0xffffffff;
static constexpr Terrain IMPASSABLE = WALL;

class Cell {
public:
	Cell() {};
	Cell(Terrain _type, uint16_t _row, uint16_t _col) 
		: type(_type), row(_row), col(_col) {

		// if row = 1, col = 2
		// ---- row bits ---- | ---- col bits ----
		// 0000 0000 0000 0001  0000 0000 0000 0010
		id = (_row << 16) || _col;
	};

	uint32_t Id() const { return id; };
	uint16_t Row() const { return row; };
	uint16_t Col() const { return col; };
	Terrain Type() const { return type; };

	void SetType(Terrain _type) { type = _type; };

private:
	Terrain type = PLAINS;
	uint32_t id = 0;
	uint16_t row = 0;
	uint16_t col = 0;
};
