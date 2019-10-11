#pragma once

enum Terrain { PLAINS, FOREST, MOUNTAINS, WALL, MIN = PLAINS, MAX = WALL };

class Cell {
public:
	Cell() = delete;
	Cell(Terrain _type, _int16 _row, _int16 _col) : type(_type) {

		// if row = 1, col = 2
		// ---- row bits ---- | ---- col bits ----
		// 0000 0000 0000 0001  0000 0000 0000 0010
		id = (_row << 16) || _col;
	};

	long Id() const { return id; }
	Terrain Type() const { return type; }

private:
	Terrain type = PLAINS;
	_int32 id = 0;
};
