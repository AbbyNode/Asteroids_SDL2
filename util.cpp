#include "util.h"

#include <math.h>

namespace util {
	float speed(float x, float y) {
		return (float)sqrt(pow(x, 2) + pow(y, 2));
	}
}