// coc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include "Vector3.h"
#include "Point.h"
#include "QuadTree.h"
#include "ConsoleDrawUtils.h"
#include <time.h>

const int MAX_RANGE = 5;
const int CONE_WIDTH = 60; // degrees

void gotHit(Vector3 magePos, Vector3 facingDir, Vector3 maybeVictimPos) {
	// check if within range
	Vector3 difference = maybeVictimPos - magePos;
	float distance = difference.magnitude();

	if (distance > MAX_RANGE) {
		std::cout << "too far away! distance = " << distance << std::endl;
		return;
	}

	// normalize the difference to get the direction of the baddie
	Vector3 directionOfBad = difference.normalize();

	// get dot product between direction of baddie and the direction mage is facing
	float dot = facingDir.dot(directionOfBad);

	// we will need to split the cone's width in half because we want to compare
	// the dot product to the cosine of the angle from the center of the cone

	// we will need to convert to radians
	auto angle = cos((CONE_WIDTH / 2) * (3.14159 / 180));
	std::cout << angle << " " << dot << std::endl;

	if (dot >= angle) {
		std::cout << "it's a hit!" << std::endl;
	}
	else {
		std::cout << "not in the cone =(" << std::endl;
	}
}


int main()
{
	//auto meAMage = Vector3(0, 0, 0);
	//auto facingDir = Vector3(0, 0, 1);

	//auto player1 = Vector3(1, 4, 6);
	//auto player2 = Vector3(-.5, 0, 2);
	//auto player3 = Vector3(.5, 0, 1.5);


	//gotHit(meAMage, facingDir, player1);
	//gotHit(meAMage, facingDir, player2);
	//gotHit(meAMage, facingDir, player3);

	static const int MAX_X = 50;
	static const int MAX_Y = 50;

	srand(time(NULL));

	QuadTree area(Point(0, 0), Point(MAX_X, MAX_Y));

	static const char* const s_charNames[] = { "Skrappy", "Chaosity", "Leayanne", "Ragin", "Liandri", "Zedd", "Skrappy", "Chaosity", "Leayanne", "Ragin", "Liandri", "Zedd", "Skrappy", "Chaosity", "Leayanne", "Ragin", "Liandri", "Zedd" };
	static const int NUM_NAMES = 18;

	std::vector<Player> players;
	for (int i = 0; i < NUM_NAMES; ++i)
	{
		players.emplace_back(Point(rand() % MAX_X, rand() % MAX_Y), s_charNames[i]);
	}

	/* Player player1(Point(4, 3), "Skrappy");
	Player player2(Point(4, 3), "Chaosity");
	Player player3(Point(4, 3), "Leayanne");
	Player player4(Point(4, 3), "Ragin");
	Player player5(Point(4, 3), "Liandri");
	Player player6(Point(4, 3), "Zedd"); */

	for (Player curPlayer : players)
	{
		area.insertPlayer(curPlayer);
	}

	/* area.insertPlayer(&player1);
	area.insertPlayer(&player2);
	area.insertPlayer(&player3);
	area.insertPlayer(&player4);
	area.insertPlayer(&player5);
	area.insertPlayer(&player6); */

	Player positionOfHit = Player(Point(4, 3), "Alaunius");
	//std::cout << positionOfHit.Name() << " got hit with chain lightning! who's next??\n";

	auto totalJumps = 4;
	auto currentJump = 1;

	//while (currentJump != totalJumps) {
	//	auto found = area.findPlayer(positionOfHit);

	//	if (found == nullptr) {
	//		//std::cout << "no one near you =(\n";
	//		//std::cout << "total jumps made = " << currentJump;
	//		break;
	//	}

	//	++currentJump;
	//	//std::cout << found->Name() << " dun been hit!\n";
	//	//std::cout << "now jumping to player closest to " << found->Position().X() << ", " << found->Position().Y() << "\n";
	//	positionOfHit = Player(found->Position(), found->Name());
	//}

	area.Draw();
	for (const Player& player : players) {
		DrawPosToConsole(player.Position().X(), player.Position().Y(), player.Name().front());
	}
	DrawPosToConsole(positionOfHit.Position().X(), positionOfHit.Position().Y(), positionOfHit.Name().front());

	std::cin.ignore();

    return 0;
}

