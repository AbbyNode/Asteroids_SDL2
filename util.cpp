#include "util.h"

#include <cmath>

namespace util {
	static const double PI = atan(1) * 4;
	static const double DEG_TO_RAD_MULT = PI / 180;

	float hypotenuse(float x, float y) {
		return (float)sqrt(pow(x, 2) + pow(y, 2));
	}

	float toZero(float value, float change) {
		float newValue = value + change;

		if ((newValue * value) < 0) { // diff signs, crossed 0
			newValue = 0;
		}
		else {
			if (newValue >= 0) { // positive
				if (newValue > value) { // went the wrong way (up)
					newValue = value;
				}
			}
			else { // negative
				if (newValue < value) { // went the wrong way (down)
					newValue = value;
				}
			}
		}

		return newValue;
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

	float warpAngle(float angle) {
		return warpValue(angle, 0, 360);
	}

	void coordFromAngle(float angle, float& x, float& y) {
		float angleRad = angle * DEG_TO_RAD_MULT;
		x = cos(angleRad);
		y = sin(angleRad);
	}
}