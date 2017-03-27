#pragma once

#include <memory>
#include <stdint.h>
#include <vector>
#include <map>

#include "../grid.h"
#include "../game.h"
#include "gene.h"
#include "neuron.h"
#include "outputs.h"

struct MutationRates {
	float connections;
	float link;
	float bias;
	float node;
	float enable;
	float disable;
	float step;
};

class Genome {
public:
	void GenerateNetwork();

	void EvaluateNetwork(const GridData& input);
	
	float GetOutputStatus(Outputs output) const;

	std::vector<std::shared_ptr<Gene>> mGenes;

	int32_t mFitness;

	int32_t mAdjustedFitness;

	std::map<int32_t, Neuron> mNetwork;

	int32_t mMaxNeuron;

	int32_t mGlobalRank;

	MutationRates mMutationRates;

	Game mGame;
};
