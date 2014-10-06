#pragma once

#include <random>

struct RandomGen {
private:
    static std::mt19937_64 rng;
public:

	static void rseed();
    static void seed(uint64_t new_seed);

    static uint64_t get();

    // inclusive, inclusive
    static float get(float min, float max);
};
