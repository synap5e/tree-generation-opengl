#pragma once

#include <random>

struct RandomGen {
private:
    static std::mt19937_64 rng;
public:

	static void rseed(){
		std::random_device rd;
		rng.seed(rd());
	}

    static void seed(uint64_t new_seed = std::mt19937_64::default_seed) {
        rng.seed(new_seed);
    }

    static uint64_t get() {
        return rng();
    }

    // inclusive, inclusive
    static float get(float min, float max){
    	// we could use a static uniform_real_distribution then change the param but this should be fine
    	std::uniform_real_distribution<float> unif(min, max);
    	return unif(rng);
    }
};
