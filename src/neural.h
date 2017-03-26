#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <memory>

#include "grid.h"
#include "game.h"

// Conditions
// Need to drop a line at least 2 times a second.
// Gets the highest points

class NeuralAi {
public:

private:
};

struct MutationRates {
	float connections;
	float link;
	float bias;
	float node;
	float enable;
	float disable;
	float step;
};

struct Gene {
	int32_t mInto;
	int32_t mOut;
	int32_t mWeight;
	bool mEnabled;
	int32_t mInnovation;
};

struct Neuron {
	std::vector<std::shared_ptr<Gene>> mIncoming;
	float mValue;
};

struct Network {
	std::map<int32_t, Neuron> mNeurons;
};

struct Genome {
	void GenerateNetwork();

	void EvaluateNetwork(const GridData& input);

	// List of genes
	std::vector<std::shared_ptr<Gene>> mGenes;

	int32_t mFitness;

	int32_t mAdjustedFitness;

	Network mNetwork;

	int32_t mMaxNeuron;

	int32_t mGlobalRank;

	MutationRates mMutationRates;

	Game mGame;
};

class Species {
public:

private:
	// Members of species
	// ???

	int32_t mStaleness;

	std::vector<Genome> mGenomes;

	int32_t mAverageFitness;

	int32_t mTopFitness;
};

class Pool {
public:
	

private:
	std::vector<Species> mSpecies;
	int32_t mGeneration;
	int32_t mCurrentSpecies;
	int32_t mCurrentFrame;
	int32_t mMaxFitness;

};