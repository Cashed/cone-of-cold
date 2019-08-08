#pragma once

#include "Point.h"
#include "Player.h"
#include <vector>

#define ENABLE_PRINT 1

class QuadTree {
	public:
		QuadTree(Point _upperLeft, Point _lowerRight)
			: upperLeft(_upperLeft), lowerRight(_lowerRight) {};

		QuadTree() : upperLeft({ 0,0 }), lowerRight({ 0,0 }) {};

		QuadTree *UpperLeftTree() const { return upperLeftTree; };
		QuadTree *UpperRightTree() const { return upperRightTree; };
		QuadTree *LowerLeftTree() const { return lowerLeftTree; };
		QuadTree *LowerRightTree() const { return lowerRightTree; };

		bool withinQuad(const Player &player);
		bool insertPlayer(Player &player);
		std::vector<Player> findNearestPlayers(Player &origin, int range);

		void Draw() const;

	private:
		Point upperLeft;
		Point lowerRight;

		std::vector<Player> players;

		QuadTree *upperLeftTree = nullptr;
		QuadTree *upperRightTree = nullptr;
		QuadTree *lowerLeftTree = nullptr;
		QuadTree *lowerRightTree = nullptr;
};