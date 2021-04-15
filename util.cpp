#include "util.h"

#include <cmath>

namespace util {
	static const double PI = atan(1) * 4;
	static const double DEG_TO_RAD_MULT = PI / 180;

	float speed(float x, float y) {
		return (float)sqrt(pow(x, 2) + pow(y, 2));
	}

	float warpValue(float val, float min, float max) {
		while (val > max) {
			val -= max;
		}
		while (val < min) {
			val += max;
		}
		return val;
	}

	float simplifyAngle(float angle) {
		return warpValue(angle, 0, 360);
	}

	void coordFromAngle(float angle, float& x, float& y) {
		float angleRad = angle * DEG_TO_RAD_MULT;
		x = cos(angleRad);
		y = sin(angleRad);
	}
}