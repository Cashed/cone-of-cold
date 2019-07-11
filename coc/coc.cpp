// coc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include "Vector3.h"
#include "Point.h"
#include "QuadTree.h"

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

	QuadTree area(Point(0, 0), Point(20, 20));

	Player player1(Point(4, 3), "Skrappy");
	Player player2(Point(4, 3), "Chaosity");
	Player player3(Point(4, 3), "Leayanne");
	Player player4(Point(4, 3), "Ragin");
	Player player5(Point(4, 3), "Liandri");
	Player player6(Point(4, 3), "Zedd");

	area.insertPlayer(&player1);
	area.insertPlayer(&player2);
	area.insertPlayer(&player3);
	area.insertPlayer(&player4);
	area.insertPlayer(&player5);
	area.insertPlayer(&player6);

	Player positionOfHit = Player(Point(4, 3), "Alaunius");
	std::cout << positionOfHit.Name() << " got hit with chain lightning! who's next??\n";

	auto totalJumps = 4;
	auto currentJump = 1;

	while (currentJump != totalJumps) {
		auto found = area.findPlayer(positionOfHit);

		if (found == nullptr) {
			std::cout << "no one near you =(\n";
			std::cout << "total jumps made = " << currentJump;
			break;
		}

		++currentJump;
		std::cout << found->Name() << " dun been hit!\n";
		std::cout << "now jumping to player closest to " << found->Position().X() << ", " << found->Position().Y() << "\n";
		positionOfHit = Player(found->Position(), found->Name());
	}

    return 0;
}

