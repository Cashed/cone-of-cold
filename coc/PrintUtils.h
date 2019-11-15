#pragma once

#include <vector>

#include "Player.h"
#include "MathUtils.cpp"

#if ENABLE_PRINT
#define PRINT(x) std::cout << x;
#else
#define DRAW_ENABLE
#define PRINT(X) void()
#endif

void printPlayerDistance(const std::vector<Player> players, const Player& initialTarget);