#include "stdafx.h"

#include "MathUtils.h"

float sqrdDistance(int firstX, int firstY, int secondX, int secondY) {
	return float((secondX - firstX) * (secondX - firstX)) + ((secondY - firstY) * (secondY - firstY));
}

float distance(Point& first, Point& second) {
	return float(sqrt(sqrdDistance(first.X(), first.Y(), second.X(), second.Y())));
}