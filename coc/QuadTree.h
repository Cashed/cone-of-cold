#pragma once

#include "Point.h"
#include "Player.h"

class QuadTree {
	public:
		QuadTree(Point _upperLeft, Point _lowerRight)
			: upperLeft(_upperLeft), lowerRight(_lowerRight) {};

		QuadTree() {
			upperLeft = Point(0, 0);
			lowerRight = Point(0, 0);
		}

		QuadTree *UpperLeftTree() const { return upperLeftTree; };
		QuadTree *UpperRightTree() const { return upperRightTree; };
		QuadTree *LowerLeftTree() const { return lowerLeftTree; };
		QuadTree *LowerRightTree() const { return lowerRightTree; };

		bool withinRange(Point &playerPos);
		void insertPlayer(const Player *player);
		Player* findPlayer(Point &playerPos);

	private:
		Point upperLeft;
		Point lowerRight;

		Player *player = NULL;

		QuadTree *upperLeftTree = NULL;
		QuadTree *upperRightTree = NULL;
		QuadTree *lowerLeftTree = NULL;
		QuadTree *lowerRightTree = NULL;
};