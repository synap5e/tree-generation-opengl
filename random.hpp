#pragma once

#include <random>
#ifdef _MSC_VER

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#endif

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
