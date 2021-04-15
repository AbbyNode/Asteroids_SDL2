#pragma once

namespace util {
	extern const double PI;
	extern const double DEG_TO_RAD_MULT;

	float speed(float x, float y);

	float warpValue(float val, float min, float max);

	float simplifyAngle(float angle);
	
	void coordFromAngle(float angle, float& x, float& y);
}