#pragma once

#include "Point.h"
#include <string>

class Player {
	public:
		Player() = delete;
		Player(const Point &_position, const std::string &_name, int _hp)
			: position(_position), name(_name), hp(_hp) {};

		Point Position() const { return position; };
		std::string Name() const { return name; };

		void lowerHp(int dmg) {
			if ((hp - dmg) < 0) {
				hp = 0;
			} else {
				hp = hp - dmg;
			}
		}
	private:
		Point position{ 0, 0 };
		std::string name;
		int hp = 100;
};