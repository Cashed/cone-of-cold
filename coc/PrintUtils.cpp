#include "stdafx.h"

#include "PrintUtils.h"

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