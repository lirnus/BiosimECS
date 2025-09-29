
#include <random>

#include "Random.h"

namespace bs {

	// Random struct
	Random::Random() : engine(std::random_device{}()), dist_uint(0, UINT32_MAX), dist_real(0, 1) {}

	uint32_t Random::getRandom32b() {
		return dist_uint(engine);
	}

	uint16_t Random::getRandom16b() {
		return static_cast<uint16_t>(dist_uint(engine));
	}

	uint8_t Random::getRandom8b() {
		return static_cast<uint8_t>(dist_uint(engine));
	}

	double Random::getRandom01() {
		return dist_real(engine);
	}

	int Random::getRandomCustom(int min, int max) {
		std::uniform_real_distribution<double> d(min, max);
		return d(engine);
	}

	int Random::getRandomIntCustom(int min, int max) {
		std::uniform_int_distribution<int> d(min, max);
		return d(engine);
	}
}
