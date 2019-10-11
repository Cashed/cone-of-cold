#include "stdafx.h"

float sqrdDistance(int firstX, int firstY, int secondX, int secondY) {
	return float((secondX - firstX) * (secondX - firstX)) + ((secondY - firstY) * (secondY - firstY));
}