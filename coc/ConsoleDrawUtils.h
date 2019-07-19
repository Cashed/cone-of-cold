#pragma once

#include <unordered_set>
#include "QuadTree.h"
#include <Windows.h>

// Util Functions for drawing in the console.

// Draw drawChar at x, y in the console,
void DrawPosToConsole(short x, short y, const char drawChar, int color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

// Draw a Box in the Console with the provided bounds.
void DrawBoxToConsole(int minX, int maxX, int minY, int maxY);
