#pragma once  

#include "stem.hpp"

#include <vector>

// see http://en.wikipedia.org/wiki/Plant_stem for terminology
// a bud grows from a node and grows to become some form of stem
class Bud : public Stem { 
private:
	Stem* grown_into;
public:
	Bud();

};
