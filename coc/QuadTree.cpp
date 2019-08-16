#include "stdafx.h"

#include "QuadTree.h"
#include "ConsoleDrawUtils.h"
#include <iostream>
#include <cmath>
#include <time.h>
#include <algorithm>

#if ENABLE_PRINT
	#define PRINT(x) std::cout << x;
#else
	#define PRINT(X) void()
#endif

void QuadTree::Draw() const
{
	DrawBoxToConsole(upperLeft.X(), lowerRight.X(), upperLeft.Y(), lowerRight.Y());

	if (upperLeftTree)
		upperLeftTree->Draw();
	if (upperRightTree)
		upperRightTree->Draw();
	if (lowerLeftTree)
		lowerLeftTree->Draw();
	if (lowerRightTree)
		lowerRightTree->Draw();
}

// we need to check that the player is within
// this quadtree by comparing player position to
// the boundries of the quadtree
bool QuadTree::withinQuad(const Player &player) {
	return player.Position().X() >= upperLeft.X()
		&& player.Position().X() <= lowerRight.X()
		&& player.Position().Y() >= upperLeft.Y()
		&& player.Position().Y() <= lowerRight.Y();
}


bool QuadTree::insertPlayer(Player &player) {
	// first check that this player even belongs
	// in this quadrant
	if (!withinQuad(player)) {
		return false;
	}
	// if this quad is not full and there are no children quadtrees, insert
	if (players.size() < 4 && upperLeftTree == nullptr) {
		players.emplace_back(player);
		return true;
	}
	
	// if this quad is full and no children, lets make some
	if (upperLeftTree == nullptr) {
		upperLeftTree = new QuadTree(
			Point(upperLeft.X(), upperLeft.Y()),
			Point(
			(upperLeft.X() + lowerRight.X()) / 2,
				(upperLeft.Y() + lowerRight.Y()) / 2
			),
			this
		);

		lowerLeftTree = new QuadTree(
			Point(upperLeft.X(), (upperLeft.Y() + lowerRight.Y()) / 2),
			Point((upperLeft.X() + lowerRight.X()) / 2, lowerRight.Y()),
			this
		);


		upperRightTree = new QuadTree(
			Point((upperLeft.X() + lowerRight.X()) / 2, upperLeft.Y()),
			Point(lowerRight.X(), (upperLeft.Y() + lowerRight.Y()) / 2),
			this
		);


		lowerRightTree = new QuadTree(
			Point(
			(upperLeft.X() + lowerRight.X()) / 2,
				(upperLeft.Y() + lowerRight.Y()) / 2
			),
			Point(lowerRight.X(), lowerRight.Y()),
			this
		);
	}

	// since we are making a new quadtree, we need to move
	// all of the players on this into the new tree
	for (auto &p : players) {
		upperLeftTree->insertPlayer(p);
		upperRightTree->insertPlayer(p);
		lowerLeftTree->insertPlayer(p);
		lowerRightTree->insertPlayer(p);
	}

	players.clear();

	// now lets try to insert the player into one of them
	if (upperLeftTree->insertPlayer(player)) return true;
	if (upperRightTree->insertPlayer(player)) return true;
	if (lowerLeftTree->insertPlayer(player)) return true;
	if (lowerRightTree->insertPlayer(player)) return true;

	return false;
}

float sqrdDistance(Point& first, Point& second) {
	return float(abs(second.X() - first.X()) + abs(second.Y() - first.Y()));
}

float distance(Point& first, Point& second) {
	return float(sqrt(abs(second.X() - first.X()) + abs(second.Y() - first.Y())));
}

double pointLineDistance(Point &vertex1, Point &vertex2, Point &castOrigin) {
	return abs((vertex2.X() - vertex1.X()) * (vertex1.Y() - castOrigin.Y()) - 
		(vertex1.X() - castOrigin.X()) * (vertex2.Y() - vertex1.Y())) / distance(vertex1, vertex2);
}


// find if a quad intersects with the radius of the spell
bool intersects(Point &upperLeft, Point &lowerRight, Point &castOrigin, int range) {
	auto upperLeftSpell = Point(castOrigin.X() - range, castOrigin.Y() + range);
	auto lowerRightSpell = Point(castOrigin.X() + range, castOrigin.Y() - range);
	// you should draw this
	return !(upperLeftSpell.Y() < upperLeft.Y() ||
		lowerRightSpell.Y() > upperLeft.Y() ||
		upperLeftSpell.X() > lowerRight.X() ||
		lowerRightSpell.X() < upperLeft.X());
}


Player& filterNearest(std::vector<Player> potentials, Player &origin, int range) {
	auto nearest = potentials[0];
	auto minDistance = sqrdDistance(nearest.Position(), origin.Position());

	for (auto& p : potentials) {
		auto distance = sqrdDistance(nearest.Position(), origin.Position());

		if (distance < minDistance) {
			nearest = p;
			minDistance = distance;
		}
	}

	return nearest;
}


QuadTree* QuadTree::findQuadForPlayer(Player& p) {
	for (auto& p : players) {
		if (p.Name().compare(p.Name()) == 0) {
			return this;
		}
	}
	
	// left?
	if ((upperLeft.X() + lowerRight.X()) / 2 >= p.Position().X()) {
		// top?
		if ((upperLeft.Y() + lowerRight.Y()) / 2 >= p.Position().Y()) {
			if (upperLeftTree == nullptr) {
				// end of the line
				return nullptr;
			}
			// the search continues
			upperLeftTree->findQuadForPlayer(p);
			// bottom
		}
		else {
			if (lowerLeftTree == nullptr) {
				// end of the line
				return nullptr;
			}
			// the search continues
			lowerLeftTree->findQuadForPlayer(p);
		}
		// right
	}
	else {
		// top?
		if ((upperLeft.Y() + lowerRight.Y()) / 2 >= p.Position().Y()) {
			if (upperRightTree == nullptr) {
				// end of the line
				return nullptr;
			}
			// the search continues
			upperRightTree->findQuadForPlayer(p);
			// bottom
		}
		else {
			if (lowerRightTree == nullptr) {
				// end of the line
				return nullptr;
			}
			// the search continues
			lowerRightTree->findQuadForPlayer(p);
		}
	}
}


Player& QuadTree::findNearestPlayer(Player& origin, int range) {
	std::vector<Player> potentials;

	// get quad where origin player is standing
	auto quad = findQuadForPlayer(origin);

	// step up through parent of each quad that intersects with cast range and collect players
	while (quad->parent != nullptr) {
		quad = quad->parent;

		if (intersects(quad->UpperLeftTree.UpperLeft(), quad->UpperLeftTree.LowerRight(), origin.Position(), range)) {
			for (auto& p : quad->UpperLeftTree.players) {
				potentials.emplace_back(p);
			}
		}


		quad = quad->parent;
	}

	// filter closest player
	return filterNearest(potentials, origin, range);
}
