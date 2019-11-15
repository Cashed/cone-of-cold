#include "stdafx.h"

#include "PlayerUtils.h"

void hitPlayer(Player& player) {
	auto dmg = rand() % 100 + 1;

	player.lowerHp(dmg);
	PRINT(player.Name());
	PRINT(" has been hit for ");
	PRINT(dmg);
	PRINT("\n");
}

void createPlayerList(int numPlayers, std::vector<Player>& players, const char *playerNames[], int maxBoundaryX, int maxBoundaryY) {
	for (int i = 0; i < numPlayers; ++i)
	{
		players.emplace_back(Point(rand() % maxBoundaryX, rand() % maxBoundaryY), playerNames[i], 100);

	}
}