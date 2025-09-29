#include <random>

#pragma once

namespace bs {
	struct Random {
		std::uniform_int_distribution<uint32_t> dist_uint;
		std::uniform_real_distribution<double> dist_real;
		std::mt19937 engine;

		Random();
		~Random() = default;

		uint32_t getRandom32b();
		uint16_t getRandom16b();
		uint8_t getRandom8b();
		double getRandom01();
		int getRandomCustom(int min, int max);
		int getRandomIntCustom(int min, int max);
	};
}
