#pragma once

#include "Player.h"

#include "PrintUtils.h"


void hitPlayer(Player& player);
void createPlayerList(int numPlayers, std::vector<Player>& players, const char *playerNames[], int maxBoundaryX, int maxBoundaryY);