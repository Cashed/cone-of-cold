#pragma once

class Point {
	public:
		Point(int _x, int _y) : x(_x), y(_y) {};
		Point() { x = 0; y = 0; };
		int X() const { return x; };
		int Y() const { return y; };
	private:
		int x = 0;
		int y = 0;
};
