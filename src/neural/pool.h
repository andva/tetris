#pragma once

#include <vector>

class Pool {
public:
	
private:
	std::vector<Species> mSpecies;
	int32_t mGeneration;
	int32_t mCurrentSpecies;
	int32_t mCurrentFrame;
	int32_t mMaxFitness;

};
