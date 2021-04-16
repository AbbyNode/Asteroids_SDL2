#include "util.h"

#include <cmath>
#include <random>

namespace util {
	const double PI = atan(1) * 4;
	const double DEG_TO_RAD_MULT = PI / 180;

	std::mt19937* randomEngine;

	//

	float hypotenuse(float x, float y) {
		return (float)sqrt(pow(x, 2) + pow(y, 2));
	}

	float distance(float x1, float y1, float x2, float y2) {
		return (float)sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
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
			val -= (max - min);
		}
		while (val < min) {
			val += (max - min);
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

	//

	void initRandom(unsigned seed) {
		randomEngine = new std::mt19937(seed);
	}

	int randomInt(int min, int max) {
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(*randomEngine);
	}

	float randomFloat(float min, float max) {
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(*randomEngine);
	}
}