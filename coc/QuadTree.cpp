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
bool QuadTree::withinRange(const Player &player) {
	return player.Position().X() >= upperLeft.X()
		&& player.Position().X() <= lowerRight.X()
		&& player.Position().Y() >= upperLeft.Y()
		&& player.Position().Y() <= lowerRight.Y();
}


bool QuadTree::insertPlayer(Player &player) {
	// first check that this player even belongs
	// in this quadrant
	if (!withinRange(player)) {
		PRINT("player out of bounds of this quad\n");
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
//
//Player* QuadTree::findPlayer(const Player &seeker) {
//	// first disqualify this quadtree!!
//	if (!withinRange(seeker)) { 
//		return nullptr;
//	}
//
//	// are you the one?? ... and also not the seeker lol
//	//if (playerNode != nullptr && strcmp(playerNode->Name().c_str(), seeker.Name().c_str()) != 0) {
//	//	PRINT("found player\n");
//	//	return playerNode;
//	//}
//
//	// now we can start the great search...
//	
//	// left?
//	if ((upperLeft.X() + lowerRight.X()) / 2 >= seeker.Position().X()) {
//		// top?
//		if ((upperLeft.Y() + lowerRight.Y()) / 2 >= seeker.Position().Y()) {
//			if (upperLeftTree == nullptr) {
//				// end of the line
//				return nullptr;
//			}
//			// the search continues
//			upperLeftTree->findPlayer(seeker);
//		// bottom
//		} else {
//			if (lowerLeftTree == nullptr) {
//				// end of the line
//				return nullptr;
//			}
//			// the search continues
//			lowerLeftTree->findPlayer(seeker);
//		}
//	// right
//	} else {
//		// top?
//		if ((upperLeft.Y() + lowerRight.Y()) / 2 >= seeker.Position().Y()) {
//			if (upperRightTree == nullptr) {
//				// end of the line
//				return nullptr;
//			}
//			// the search continues
//			upperRightTree->findPlayer(seeker);
//		// bottom
//		} else {
//			if (lowerRightTree == nullptr) {
//				// end of the line
//				return nullptr;
//			}
//			// the search continues
//			lowerRightTree->findPlayer(seeker);
//		}
//	}
//}