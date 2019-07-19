#pragma once

class Point {
	public:
		Point() {};
		Point(int _x, int _y) : x(_x), y(_y) {};
		int X() const { return x; };
		int Y() const { return y; };
	private:
		int x = 0;
		int y = 0;
};
