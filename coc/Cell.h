#pragma once

enum Terrain { PLAINS, FOREST, MOUNTAINS, WATER, WALL, MIN = PLAINS, MAX = WALL };

class Cell {
public:
	Cell() {};
	Cell(Terrain _type) : type(_type) {};

	Terrain Type() const { return type; }
private:
	Terrain type = PLAINS;
};