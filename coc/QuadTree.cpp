#include "stdafx.h"

#include "QuadTree.h"
#include "ConsoleDrawUtils.h"
#include <iostream>
#include <cmath>
#include <time.h>
#include <algorithm>
#include <stack>

#if ENABLE_PRINT
	#define PRINT(x) std::cout << x;
#else
	#define PRINT(X) void()
#endif

void QuadTree::Draw() const
{
	DrawBoxToConsole(upperLeft.X(), lowerRight.X(), upperLeft.Y(), lowerRight.Y());

	for (const auto& player : players)
	{
		DrawPosToConsole(static_cast<short>(player.Position().X()), static_cast<short>(player.Position().Y()), player.Name()[0]);
	}

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

// find if a quad intersects with the radius of the spell
bool intersects(Point &upperLeft, Point &lowerRight, Point &castOrigin, int range) {
	auto upperLeftSpell = Point(castOrigin.X() - range, castOrigin.Y() + range);
	auto lowerRightSpell = Point(castOrigin.X() + range, castOrigin.Y() - range);
	// you should draw this
	return !(lowerRightSpell.Y() < upperLeft.Y() ||
		upperLeftSpell.Y() < lowerRight.Y() ||
		upperLeftSpell.X() > lowerRight.X() ||
		lowerRightSpell.X() < upperLeft.X());
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


void QuadTree::getNearestPlayers(std::vector<Player> &nearbyPlayers, const Player &origin, int range) {
	if (!intersects(upperLeft, lowerRight, origin.Position(), range)) {
		return;
	}

	if (upperLeftTree == nullptr) {
		for (auto& p : players) {
			nearbyPlayers.emplace_back(p);
		}
	}
	else {
		upperLeftTree->getNearestPlayers(nearbyPlayers, origin, range);
		lowerLeftTree->getNearestPlayers(nearbyPlayers, origin, range);
		upperRightTree->getNearestPlayers(nearbyPlayers, origin, range);
		lowerRightTree->getNearestPlayers(nearbyPlayers, origin, range);
	}
}

void QuadTree::getNearestPlayersIter(std::vector<Player>& nearbyPlayers, const Player& origin, int range) {
	std::stack<QuadTree*> stack;

	stack.emplace(this);

	while (!stack.empty()) {
		auto current = stack.top();
		stack.pop();

		if (!intersects(current->upperLeft, current->lowerRight, origin.Position(), range)) {
			continue;
		}

		if (current->upperLeftTree != nullptr) {
			stack.emplace(current->upperLeftTree);
			stack.emplace(current->upperRightTree);
			stack.emplace(current->lowerLeftTree);
			stack.emplace(current->lowerRightTree);
		}
		else {
			for (auto& p : current->players) {
				nearbyPlayers.emplace_back(p);
			}
		}
	}
}