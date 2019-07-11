#include "stdafx.h"
#include <cmath>
#include "Vector3.h"

Vector3 Vector3::operator-(const Vector3 &v) {
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator/(const float &f) {
	return Vector3(x / f, y / f, z / f);
}

float Vector3::sqrMagnitude() {
	return x * x + y * y + z * z;
}

float Vector3::magnitude() {
	return std::sqrt(sqrMagnitude());
}

Vector3 Vector3::normalize() {
	return Vector3(x, y, z) / magnitude();
}

float Vector3::dot(const Vector3 &v) {
	return (x * v.x) + (y * v.y) + (z * v.z);
}