#pragma once

#include <stdint.h>

namespace utils {

	namespace rng {

		static uint32_t s_rng_seed = 2463534242;

		inline uint32_t XorShift32() {
			s_rng_seed ^= s_rng_seed << 13;
			s_rng_seed ^= s_rng_seed >> 17;
			s_rng_seed ^= s_rng_seed << 5;
			return s_rng_seed;
		}

		// Fast random float between 0 and 1
		inline float FastRandom() {
			return (XorShift32() & 0xFFFFFF) / 16777215.0f;  // Normalized to 0-1
		}

		inline float RandomBetween(float from, float to) {
			return from + (to - from) * FastRandom();
		}
	}
}