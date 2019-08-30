#pragma once

#include "Point.h"
#include "Player.h"
#include <vector>

#define ENABLE_PRINT 1

class QuadTree {
public:
	QuadTree(Point _upperLeft, Point _lowerRight, QuadTree* _parent)
		: upperLeft(_upperLeft), lowerRight(_lowerRight), parent(_parent) {};

	QuadTree() : upperLeft({ 0,0 }), lowerRight({ 0,0 }) {};

	QuadTree* UpperLeftTree() const { return upperLeftTree; };
	QuadTree* UpperRightTree() const { return upperRightTree; };
	QuadTree* LowerLeftTree() const { return lowerLeftTree; };
	QuadTree* LowerRightTree() const { return lowerRightTree; };

	Point UpperLeft() const { return upperLeft; };
	Point LowerRight() const { return lowerRight; };

	bool withinQuad(const Player &player);
	bool insertPlayer(Player &player);
	Player& findNearestPlayer(Player &origin, int range);
	Player& findNearestPlayer2(Player& origin, int range);
	QuadTree* findQuadForPlayer(Player& p);
	void getNearbyPlayers(std::vector<Player>& potentials);

	void Draw() const;

	private:
		Point upperLeft;
		Point lowerRight;

		std::vector<Player> players;

		QuadTree *parent = nullptr;

		QuadTree *upperLeftTree = nullptr;
		QuadTree *upperRightTree = nullptr;
		QuadTree *lowerLeftTree = nullptr;
		QuadTree *lowerRightTree = nullptr;
};