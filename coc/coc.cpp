// coc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include "Vector3.h"

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
	auto meAMage = Vector3(0, 0, 0);
	auto facingDir = Vector3(0, 0, 1);

	auto player1 = Vector3(1, 4, 6);
	auto player2 = Vector3(-.5, 0, 2);
	auto player3 = Vector3(.5, 0, 1.5);


	gotHit(meAMage, facingDir, player1);
	gotHit(meAMage, facingDir, player2);
	gotHit(meAMage, facingDir, player3);

	system("pause");
    return 0;
}

