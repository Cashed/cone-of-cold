#pragma once
#include <iostream>
#include <algorithm>

#include "ConsoleDrawUtils.h"
#include "ChainLightning.h"
#include "PrintUtils.h"
#include "PlayerUtils.h"

#include "Player.h"
#include "QuadTree.h"

bool isDuplicate(std::vector<Player>& players, const Player& player) {
	for (auto& p : players) {
		auto it = std::find_if(players.begin(), players.end(), [&player](const Player pl) { return pl.Name().compare(player.Name()) == 0; });

		if (it != players.end())
		{
			// found player
			return true;
		}
	}
	return false;
}

void castChainLightning(const Player& caster, Player& initial, QuadTree& area, int range) {
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

	auto& target = initial;

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

	for (auto& t : targets) {
		// hit player
		PRINT("\ntarget = ");
		PRINT(t.Name());
		PRINT("\n");

		hitPlayer(t);
	}

	std::cin.ignore();
}