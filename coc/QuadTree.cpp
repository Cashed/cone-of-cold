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
			)
		);

		lowerLeftTree = new QuadTree(
			Point(upperLeft.X(), (upperLeft.Y() + lowerRight.Y()) / 2),
			Point((upperLeft.X() + lowerRight.X()) / 2, lowerRight.Y())
		);


		upperRightTree = new QuadTree(
			Point((upperLeft.X() + lowerRight.X()) / 2, upperLeft.Y()),
			Point(lowerRight.X(), (upperLeft.Y() + lowerRight.Y()) / 2)
		);


		lowerRightTree = new QuadTree(
			Point(
			(upperLeft.X() + lowerRight.X()) / 2,
				(upperLeft.Y() + lowerRight.Y()) / 2
			),
			Point(lowerRight.X(), lowerRight.Y())
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
	// get the other two vertices of the quad
	auto lowerLeft = Point(upperLeft.X(), lowerRight.Y());
	auto upperRight = Point(lowerRight.X(), upperLeft.Y());

	// check if the distance between any of these vertices and
	// center of the cast is shorter than the range
	if (distance(castOrigin, upperLeft) <= range) {
		return true;
	}
	else if (distance(castOrigin, lowerLeft) <= range) {
		return true;
	}
	else if (distance(castOrigin, upperRight) <= range) {
		return true;
	}
	else if (distance(castOrigin, lowerRight) <= range) {
		return true;
	}

	// check if the distance between an edge of the quad and
	// center of the cast is shorter than the range
	// top edge
	if (pointLineDistance(upperLeft, upperRight, castOrigin) <= range) {
		return true;
	}
	// left edge
	else if (pointLineDistance(upperLeft, lowerLeft, castOrigin) <= range) {
		return true;
	}
	// bottom edge
	else if (pointLineDistance(lowerLeft, lowerRight, castOrigin) <= range) {
		return true;
	}
	// right edge
	else if (pointLineDistance(upperRight, lowerRight, castOrigin) <= range) {
		return true;
	}

	return false;
}


std::vector<Player> QuadTree::findNearestPlayers(Player &origin, int range) {
	auto radius = 40;
	std::vector<Player> potentials;

	// check if origin of the cast lies within this quad
	// or if the quad intersects within the range of the cast
	if (withinQuad(origin) || intersects(upperLeft, lowerRight, origin.Position(), range)) {
		for (auto &p : players) {
			potentials.emplace_back(p);
		}

		// if this is the end of the line, return
		if (upperLeftTree == nullptr) {
			return potentials;
		}

		// if not, keep looking down the line
		auto upperLeftPotentials = upperLeftTree->findNearestPlayers(origin, range);
		auto lowerLeftPotentials = lowerLeftTree->findNearestPlayers(origin, range);
		auto upperRightPotentials = upperRightTree->findNearestPlayers(origin, range);
		auto lowerRightPotentials = lowerRightTree->findNearestPlayers(origin, range);

		// concatentate the vectors returned
		potentials.insert(
			potentials.end(),
			std::make_move_iterator(upperLeftPotentials.begin()),
			std::make_move_iterator(upperLeftPotentials.end())
		);

		potentials.insert(
			potentials.end(),
			std::make_move_iterator(lowerLeftPotentials.begin()),
			std::make_move_iterator(lowerLeftPotentials.end())
		);

		potentials.insert(
			potentials.end(),
			std::make_move_iterator(upperRightPotentials.begin()),
			std::make_move_iterator(upperRightPotentials.end())
		);

		potentials.insert(
			potentials.end(),
			std::make_move_iterator(lowerRightPotentials.begin()),
			std::make_move_iterator(lowerRightPotentials.end())
		);
	}

	return potentials;
}
