#pragma once

#include <vector>
#include <iostream>

#include "Player.h"
#include "QuadTree.h"


void castChainLightning(const Player& caster, Player& initial, QuadTree& area, int range);
bool isDuplicate(std::vector<Player>& players, const Player& player);