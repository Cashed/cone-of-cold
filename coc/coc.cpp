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
#include <algorithm>

#include "utils.cpp"

#if ENABLE_PRINT
#define PRINT(x) std::cout << x;
#else
#define DRAW_ENABLE
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

Player& filterNearest(std::vector<Player>& potentials, const Player& origin, int range) {
	Player& nearest = potentials[0];
	auto minDistance = INT16_MAX;

	for (auto& p : potentials) {
		auto distance = sqrdDistance(p.Position().X(), p.Position().Y(), origin.Position().X(), origin.Position().Y());

		if (distance < minDistance) {
			nearest = p;
			minDistance = distance;
		}
	}

	return nearest;
}

bool isDuplicate(std::vector<Player> &players, const Player& player) {
	for (auto &p : players) {
		auto it = std::find_if(players.begin(), players.end(), [&player](const Player pl) { return pl.Name().compare(player.Name()) == 0; });

			if (it != players.end())
			{
				// found player
				return true;
			}
	}
	return false;
}

float distance(Point& first, Point& second) {
	return float(sqrt(sqrdDistance(first.X(), first.Y(), second.X(), second.Y())));
}

void printPlayerDistance(const std::vector<Player> players, const Player& initialTarget) {
	for (const Player& p : players) {
		PRINT(p.Name());
		PRINT("(");
		PRINT(p.Position().X());
		PRINT(", ");
		PRINT(p.Position().Y());
		PRINT(") - ");
		PRINT(distance(initialTarget.Position(), p.Position()));
		PRINT(" target = ");
		PRINT(initialTarget.Name());
		PRINT("\n");
	}
}

void castChainLightning(const Player &caster, Player &initial, QuadTree &area, int range) {
	// list of nearby players
	std::vector<Player> playersNearby;
	// list of final targets
	std::vector<Player> targets;

	auto maxDistance = range * range;

	PRINT("target = ");
	PRINT(initial.Name());
	PRINT("\n");

	targets.emplace_back(initial);

	auto numJumps = 4;
	
	auto &target = initial;

	// keep finding nearest player till jumps are exhausted
	while (numJumps > 0) {
#ifdef DRAW_ENABLE
		area.Draw(); // Redraw the area to clear coloring
#endif

		// get all players nearby target
		area.getNearestPlayersIter(playersNearby, target, range);

		//filter for duplicates, players already hit, initial target
		if (playersNearby.empty()) {
			PRINT("No additional players nearby.");
			numJumps = 0;
			continue;
		}

		Player& nearest = playersNearby[0];
		auto minDistance = INT16_MAX;
		for (auto& p : playersNearby) {
#ifdef DRAW_ENABLE
			DrawPosToConsole(static_cast<short>(p.Position().X()), static_cast<short>(p.Position().Y()), p.Name()[0], FOREGROUND_RED); // Considered Targets
#endif
			if (p.Name().compare(caster.Name()) != 0 && !isDuplicate(targets, p)) {
				auto distance = sqrdDistance(p.Position().X(), p.Position().Y(), target.Position().X(), target.Position().Y());
				PRINT(p.Name());
				PRINT(" ");
				PRINT(distance);
				PRINT(" min ");
				PRINT(minDistance);
				PRINT("\n");
				if (distance < minDistance) {
					PRINT(p.Name());
					PRINT("distance is less");
					PRINT("\n");
					nearest = p;
					minDistance = distance;
				}
			}
		}

		printPlayerDistance(playersNearby, target);
		PRINT("Current min distance is ");
		PRINT(minDistance);
		PRINT(" max is ");
		PRINT(maxDistance);
		PRINT("\n");

		if (minDistance > maxDistance) {
			PRINT("No additional players nearby after distance check.");
			numJumps = 0;
			continue;
		}

#ifdef DRAW_ENABLE
		DrawPosToConsole(static_cast<short>(target.Position().X()), static_cast<short>(target.Position().Y()), target.Name()[0], FOREGROUND_BLUE); // Start Target
		DrawPosToConsole(static_cast<short>(nearest.Position().X()), static_cast<short>(nearest.Position().Y()), nearest.Name()[0], FOREGROUND_GREEN); // Selected Target
#endif
		targets.emplace_back(nearest);

		// move on to next
		target = nearest;
		playersNearby.clear();
		--numJumps;
#ifdef DRAW_ENABLE
		std::cin.ignore();
#endif
	}

	for (auto &t : targets) {
		// hit player
		PRINT("\ntarget = ");
		PRINT(t.Name());
		PRINT("\n");

		hitPlayer(t);
	}

	std::cin.ignore();
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

	static const int MAX_X = 80;
	static const int MAX_Y = 80;

	srand(time(NULL));

	QuadTree area(Point(0, 0), Point(MAX_X, MAX_Y), nullptr);

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


	Player &initialTarget = players[rand() % 10];
	Player &caster = players[rand() % 18];

	while (initialTarget.Name().compare(caster.Name()) == 0) {
		caster = players[rand() % 17];
	}

	PRINT(caster.Name());
	PRINT(" is first targetting ");
	PRINT(initialTarget.Name());
	PRINT("\n");

	castChainLightning(caster, initialTarget, area, 40);

	//auto targets = getTargets(3, neighbors, initialTarget);

	//for (auto& p : targets) {
	//	hitPlayer(p);
	//}

	//area.Draw();
	//for (const Player& player : players) {
	//	DrawPosToConsole(player.Position().X(), player.Position().Y(), player.Name().front());
	//}
	//DrawPosToConsole(initialTarget.Position().X(), initialTarget.Position().Y(), initialTarget.Name().front());

	//std::cin.ignore();

    return 0;
}

