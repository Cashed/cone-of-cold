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
	QuadTree* findQuadForPlayer(Player& p);
	void getNearestPlayers(std::vector<Player>& potentials, const Player &origin, int range);
	void getNearestPlayersIter(std::vector<Player>& nearbyPlayers, const Player& origin, int range);

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