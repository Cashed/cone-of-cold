#pragma once

class Vector3 {
	public:
		Vector3(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z) {};

		Vector3 operator-(const Vector3 v);
		Vector3 operator/(const float f);
		float magnitude();
		Vector3 normalize();
		float dot(const Vector3 v);
		

	private:
		float x = 0;
		float y = 0;
		float z = 0;

};