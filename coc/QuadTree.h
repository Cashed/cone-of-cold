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

		bool withinRange(const Player *player);
		void insertPlayer(Player *player);
		Player* findPlayer(const Player &seeker);

	private:
		Point upperLeft;
		Point lowerRight;

		Player *playerNode = nullptr;

		QuadTree *upperLeftTree = nullptr;
		QuadTree *upperRightTree = nullptr;
		QuadTree *lowerLeftTree = nullptr;
		QuadTree *lowerRightTree = nullptr;
};