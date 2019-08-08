// coc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include "Vector3.h"
#include "Point.h"
#include "QuadTree.h"
#include "ConsoleDrawUtils.h"
#include <time.h>

#if ENABLE_PRINT
#define PRINT(x) std::cout << x;
#else
#define PRINT(X) void()
#endif

const int MAX_RANGE = 5;
const int CONE_WIDTH = 60; // degrees

void gotHit(Vector3 magePos, Vector3 facingDir, Vector3 maybeVictimPos) {
	// check if within range
	Vector3 difference = maybeVictimPos - magePos;
	float distance = difference.magnitude();

	if (distance > MAX_RANGE) {
		std::cout << "too far away! distance = " << distance << std::endl;
		return;
	}

	// normalize the difference to get the direction of the baddie
	Vector3 directionOfBad = difference.normalize();

	// get dot product between direction of baddie and the direction mage is facing
	float dot = facingDir.dot(directionOfBad);

	// we will need to split the cone's width in half because we want to compare
	// the dot product to the cosine of the angle from the center of the cone

	// we will need to convert to radians
	auto angle = cos((CONE_WIDTH / 2) * (3.14159 / 180));
	std::cout << angle << " " << dot << std::endl;

	if (dot >= angle) {
		std::cout << "it's a hit!" << std::endl;
	}
	else {
		std::cout << "not in the cone =(" << std::endl;
	}
}

void hitPlayer(Player& player) {
	auto dmg = rand() % 100 + 1;

	player.lowerHp(dmg);
	PRINT(player.Name());
	PRINT(" has been hit for ");
	PRINT(dmg);
	PRINT("\n");
}

std::vector<Player> getTargets(int num, std::vector<Player> &potentials, Player &initial) {
	std::vector<Player> targets;
	std::vector<std::string> seen;

	// filter duplicates and make sure we arent hitting the initial guy
	for (auto i = 0; i < num; ++i) {
		if (potentials[i].Name() != initial.Name() && !(std::find(seen.begin(), seen.end(), potentials[i].Name()) != seen.end())) {
			targets.emplace_back(potentials[i]);
		}

		seen.emplace_back(potentials[i].Name());
	}

	return targets;
}


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

	static const int MAX_X = 50;
	static const int MAX_Y = 50;

	srand(time(NULL));

	QuadTree area(Point(0, 0), Point(MAX_X, MAX_Y));

	static const char* const s_charNames[] = { "Skrappy", "Chaosity", "Leayanne", "Ragin", "Liandri", "Zedd", "Taliana", "Flame", "Mel", "Erelia", "Cached", "Reagan", "Zimble", "Kass", "Ankou", "Chef", "Mike", "Gordon" };
	static const int NUM_NAMES = 18;

	std::vector<Player> players;
	for (int i = 0; i < NUM_NAMES; ++i)
	{
		players.emplace_back(Point(rand() % MAX_X, rand() % MAX_Y), s_charNames[i], 100);
	}

	for (Player curPlayer : players)
	{
		area.insertPlayer(curPlayer);
	}


	Player initialTarget = players[rand() % 10];

	auto neighbors = area.findNearestPlayers(initialTarget, 40);

	auto targets = getTargets(3, neighbors, initialTarget);

	hitPlayer(initialTarget);

	for (auto& p : targets) {
		hitPlayer(p);
	}

	area.Draw();
	for (const Player& player : players) {
		DrawPosToConsole(player.Position().X(), player.Position().Y(), player.Name().front());
	}
	DrawPosToConsole(initialTarget.Position().X(), initialTarget.Position().Y(), initialTarget.Name().front());

	std::cin.ignore();

    return 0;
}

