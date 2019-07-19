#include "stdafx.h"

#include "QuadTree.h"
#include "ConsoleDrawUtils.h"
#include <iostream>
#include <cmath>

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
bool QuadTree::withinRange(const Player *player) {
	return player->Position().X() >= upperLeft.X()
		&& player->Position().X() <= lowerRight.X()
		&& player->Position().Y() >= upperLeft.Y()
		&& player->Position().Y() <= lowerRight.Y();
}

void QuadTree::insertPlayer(Player *player) {
	if (player == nullptr) {
		return;
	}
	// first check that this player even belongs
	// in this quadrant
	if (!withinRange(player)) {
		PRINT("player out of bounds of this quad\n");
		return;
	}

	// we need a way to tell the recursion to stop once we reach
	// the deepest quadtree.
	// This will be the shortest range we will allow to be recognized.
	// Let's go with 7 yards
	if (std::abs(upperLeft.X() - lowerRight.X()) <= 7 
		&& std::abs(upperLeft.Y() - lowerRight.Y()) <= 7) {
		// we are at the lowest granularity.
		// the player can exist if there is not already one here
		if (playerNode == nullptr) {
			PRINT("playernode is null, time to insert!\n");
			playerNode = player;
			PRINT("player inserted\n");
			return;
		} else {
			PRINT("no room for you!\n");
			return;
		}

	}

	// now we need to find what sub quadtree within this quadtree
	// the player can be inserted

	// first check if it belongs on the left side
	if ((upperLeft.X() + lowerRight.X()) / 2 >= player->Position().X()) {
		// is it on the top side?
		if ((upperLeft.Y() + lowerRight.Y()) / 2 >= player->Position().Y()) { // y values go higher going "down"
			// if this is the first one, we will need
			// to make a new quadtree
			if (upperLeftTree == nullptr) {
				upperLeftTree = new QuadTree(
					Point(upperLeft.X(), upperLeft.Y()),
					Point(
						(upperLeft.X() + lowerRight.X()) / 2,
						(upperLeft.Y() + lowerRight.Y()) / 2
					)
				);
			}
			// recursively insert
			upperLeftTree->insertPlayer(player);
		// bottom side
		} else {
			// check if it's the first one
			if (lowerLeftTree == nullptr) {
				lowerLeftTree = new QuadTree(
					Point(upperLeft.X(), (upperLeft.Y() + lowerRight.Y()) / 2),
					Point((upperLeft.X() + lowerRight.X()) / 2, lowerRight.Y())
				);
			}
			// recursively insert
			lowerLeftTree->insertPlayer(player);
		}
	// belongs on the right side
	} else {
		// is it top side?
		if ((upperLeft.Y() + lowerRight.Y()) / 2 >= player->Position().Y()) {
			// is this the first one?
			if (upperRightTree == nullptr) {
				upperRightTree = new QuadTree(
					Point((upperLeft.X() + lowerRight.X()) / 2, upperLeft.Y()),
					Point(lowerRight.X(), (upperLeft.Y() + lowerRight.Y()) / 2)
				);
			}
			// recursively insert
			upperRightTree->insertPlayer(player);
		// bottom side
		} else {
			if (lowerRightTree == nullptr) {
				lowerRightTree = new QuadTree(
					Point(
					(upperLeft.X() + lowerRight.X()) / 2,
					(upperLeft.Y() + lowerRight.Y()) / 2
					),
					Point(lowerRight.X(), lowerRight.Y())
				);
			}
			// recursively insert
			lowerRightTree->insertPlayer(player);
		}
	}
}

Player* QuadTree::findPlayer(const Player &seeker) {
	// first disqualify this quadtree!!
	if (!withinRange(&seeker)) { 
		return nullptr;
	}

	// are you the one?? ... and also not the seeker lol
	if (playerNode != nullptr && strcmp(playerNode->Name().c_str(), seeker.Name().c_str()) != 0) {
		PRINT("found player\n");
		return playerNode;
	}

	// now we can start the great search...
	
	// left?
	if ((upperLeft.X() + lowerRight.X()) / 2 >= seeker.Position().X()) {
		// top?
		if ((upperLeft.Y() + lowerRight.Y()) / 2 >= seeker.Position().Y()) {
			if (upperLeftTree == nullptr) {
				// end of the line
				return nullptr;
			}
			// the search continues
			upperLeftTree->findPlayer(seeker);
		// bottom
		} else {
			if (lowerLeftTree == nullptr) {
				// end of the line
				return nullptr;
			}
			// the search continues
			lowerLeftTree->findPlayer(seeker);
		}
	// right
	} else {
		// top?
		if ((upperLeft.Y() + lowerRight.Y()) / 2 >= seeker.Position().Y()) {
			if (upperRightTree == nullptr) {
				// end of the line
				return nullptr;
			}
			// the search continues
			upperRightTree->findPlayer(seeker);
		// bottom
		} else {
			if (lowerRightTree == nullptr) {
				// end of the line
				return nullptr;
			}
			// the search continues
			lowerRightTree->findPlayer(seeker);
		}
	}
}