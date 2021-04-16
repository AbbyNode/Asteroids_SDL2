#pragma once

#include <stdint.h>

namespace util {
	extern const double PI;
	extern const double DEG_TO_RAD_MULT;

	float hypotenuse(float x, float y);

	float distance(float x1, float y1, float x2, float y2);

	float toZero(float value, float change);

	float warpValue(float val, float min, float max);

	float warpAngle(float angle);
	
	void coordFromAngle(float angle, float& x, float& y);

	//

	void initRandom(unsigned seed);

	int randomInt(int min, int max);

	float randomFloat(float min, float max);
}