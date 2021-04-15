#pragma once

namespace util {
	extern const double PI;
	extern const double DEG_TO_RAD_MULT;

	float hypotenuse(float x, float y);

	float toZero(float value, float change);

	float warpValue(float val, float min, float max);

	float warpAngle(float angle);
	
	void coordFromAngle(float angle, float& x, float& y);
}