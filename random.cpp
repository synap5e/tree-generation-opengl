#include "random.hpp"

std::mt19937_64 RandomGen::rng;

void RandomGen::rseed(){
	std::random_device rd;
	rng.seed(rd());
}

void RandomGen::seed(uint64_t new_seed) {
    rng.seed(new_seed);
}

uint64_t RandomGen::get() {
    return rng();
}

float RandomGen::get(float min, float max){
	// we could use a static uniform_real_distribution then change the param but this should be fine
	float a,b;
	if (min < max){
        a = min;
        b = max;
    } else {
        b = min;
        a = max;
    }
    std::uniform_real_distribution<float> unif(a,b);
	return unif(rng);
}
