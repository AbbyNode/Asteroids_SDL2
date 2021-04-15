#include "util.h"

#include <cmath>

namespace util {
	static const double PI = atan(1) * 4;
	static const double DEG_TO_RAD_MULT = PI / 180;

	float speed(float x, float y) {
		return (float)sqrt(pow(x, 2) + pow(y, 2));
	}

	float toZero(float value, float change) {
		float newValue = value + change;
		if (abs(newValue) > abs(value)) {
			// 
			/*
vel old	vel change	add	vel new			abs of new further from 0
20	5	-5	15		15
20	-5	5	25	no		20
-20	5	-5	-25	no		-20
-20	-5	5	-15		-15
20	25	-25	-5	no			0
20	-25	25	45	no		20
-20	25	-25	-45	no		-20
-20	-25	25	5	no			0
			*/
		}
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