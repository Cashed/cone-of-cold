// coc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <time.h>

#include "Vector3.h"
#include "Point.h"
#include "QuadTree.h"
#include "ConsoleDrawUtils.h"
#include "PrintUtils.h"
#include "Map.h"
#include "ChainLightning.h"
#include "ConeOfCold.h"
#include "PlayerUtils.h"

int main()
{
	//auto meAMage = Vector3(0, 0, 0);
	//auto facingDir = Vector3(0, 0, 1);

	//auto player1 = Vector3(1, 4, 6);
	//auto player2 = Vector3(-.5, 0, 2);
	//auto player3 = Vector3(.5, 0, 1.5);


	//gotHit(meAMage, facingDir, player1);
	//gotHit(meAMage, facingDir, player2);
	//gotHit(meAMage, facingDir, player3);

	Map map(20, 20);
	auto start = map.getCell(0, 0);
	auto end = map.getCell(2, 2);
	auto path = map.getPath(start.Id(), end.Id());

	std::cout << path.size() << "\n";

	for (auto& cell : path) {
		std::cout << cell << "\n";
	}

	//static const int MAX_X = 80;
	//static const int MAX_Y = 80;

	//srand(time(NULL));

	//QuadTree area(Point(0, 0), Point(MAX_X, MAX_Y), nullptr);

	//static const char* s_charNames[] = { "Skrappy", "Chaosity", "Leayanne", "Ragin", "Liandri", "Zedd", "Taliana", "Flame", "Mel", "Erelia", "Cached", "Reagan", "Zimble", "Kass", "Ankou", "Chef", "Mike", "Gordon" };
	//static const int NUM_NAMES = 18;

	//std::vector<Player> players;
	//
	//createPlayerList(NUM_NAMES, players, s_charNames, MAX_X, MAX_Y);

	//for (Player curPlayer : players)
	//{
	//	area.insertPlayer(curPlayer);
	//}


	//Player &initialTarget = players[rand() % 10];
	//Player &caster = players[rand() % 18];

	//while (initialTarget.Name().compare(caster.Name()) == 0) {
	//	caster = players[rand() % 17];
	//}

	//PRINT(caster.Name());
	//PRINT(" is first targetting ");
	//PRINT(initialTarget.Name());
	//PRINT("\n");

	//castChainLightning(caster, initialTarget, area, 40);

	//auto targets = getTargets(3, neighbors, initialTarget);

	//area.Draw();
	//for (const Player& player : players) {
	//	DrawPosToConsole(player.Position().X(), player.Position().Y(), player.Name().front());
	//}
	//DrawPosToConsole(initialTarget.Position().X(), initialTarget.Position().Y(), initialTarget.Name().front());

	//std::cin.ignore();

    return 0;
}

