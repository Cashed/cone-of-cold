#pragma once

#include "Point.h"
#include <string>

class Player {
	public:
		Player() = delete;
		Player(Point _position, std::string _name)
			: position(_position), name(_name) {};

		Point Position() const { return position; };
		std::string Name() const { return name; };
	private:
		Point position;
		std::string name;
};