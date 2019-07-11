#include "QuadTree.h";
#include <iostream>;

// we need to check that the player is within
// this quadtree by comparing player position to
// the boundries of the quadtree
bool QuadTree::withinRange(Point &playerPos) {
	return playerPos.X >= upperLeft.X 
		&& playerPos.X <= lowerRight.X
		&& playerPos.Y >= upperLeft.Y 
		&& playerPos.Y <= lowerRight.Y;
}

void QuadTree::insertPlayer(const Player *player) {
	// first check that this player even belongs
	// in this quadrant
	if (!withinRange(player->Position)) {
		std::cout << "player out of bounds of this quad\n";
		return;
	}

	// now we need to find what sub quadtree within this quadtree
	// the player can be inserted

	// first check if it belongs on the left side
	if ((upperLeft.X + lowerRight.X) / 2 >= player->Position().X) {
		// now we know its left, lets check if its on the top
		if ((upperLeft.Y + lowerRight.Y) / 2 >= player->Position().Y) {
			// if this is the first one, we will need
			// to make a new quadtree
			if (upperLeftTree == NULL) {
				upperLeftTree = new QuadTree(

				)
			}
		}


	}
}