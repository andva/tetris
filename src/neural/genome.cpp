#include "genome.h"

#include <algorithm>

#include "../piecefactory.h"
#include "neuron.h"
#include "outputs.h"

namespace {
//const int POPULATION = 300;
//const float DELTA_DIS_JOINT = 2.0f;
//const float DELTA_WEIGHTS = 0.4f;
//const float DELTA_TREASHOLD = 1.0f;
//
//const int STALE_SPECIES = 15;
//
//const float MUTATE_CONNECTIONS_CHANCE = 0.25f;
//const float PERTURB_CHANCE = 0.9f;
//const float CROSSOVER_CHANCE = 0.75f;
//const float LINK_MUTATION_CHANCE = 2.0f;
//const float NODE_MUTATION_CHANCE = 0.5f;
//const float BIAS_MUTATION_CHANCE = 0.40f;
//const float STEP_SIZE = 0.1f;
//const float DISABLE_MUTATION_CHANCE = 0.4f;
//const float ENABLE_MUTATION_CHANCE = 0.2f;
//
//// MS until species timeouts
//const float TIMEOUT_CONSTANT = 100;
//
const int MAX_NODES = 1000000;

float Sigmoid(float sum) {
	return 2.0f / (1.0f + exp(-4.9f * sum)) - 1.0f;
}
}

void Genome::GenerateNetwork()
{
	// Create input neurons for input
	for (int i = 0; i < 10 * 20; i++) {
		mNetwork[i] = Neuron();
	}

	// Create output neurons for buttons
	for (int o = 0; o < NUM_OUTPUTS; o++) {
		mNetwork[mMaxNeuron + o] = Neuron();
	}

	// Sort genes by out.
	// Why?
	std::sort(mGenes.begin(), mGenes.end(), [] (std::shared_ptr<Gene> a, std::shared_ptr<Gene> b) { return a->mOut < b->mOut; });

	// Iterate trough genes
	for (auto gene : mGenes) {
		if (gene->mEnabled) {
			if (mNetwork.find(gene->mOut) == mNetwork.end()) {
				mNetwork[gene->mOut] = Neuron();
			}
			mNetwork[gene->mOut].mIncoming.push_back(gene);

			if (mNetwork.find(gene->mInto) == mNetwork.end()) {
				mNetwork[gene->mInto] = Neuron();
			}
		}
	}
}

void Genome::EvaluateNetwork(const GridData& input)
{
	// Add input data as values to neural network
	for (int i = 0; i < 10 * 20; i++) {
		int32_t x = i % 10;
		int32_t y = static_cast<int32_t>(std::floor(static_cast<float>(i / 10)));
		mNetwork[i].mValue = static_cast<float>(input[y][x]);
	}
	for (auto& neuron : mNetwork) {
		float sum = 0;
		for (int i = 0; i < neuron.second.mIncoming.size(); i++) {
			auto incoming = neuron.second.mIncoming[i];
			auto other_incoming = mNetwork[incoming->mInto];
			sum += incoming->mWeight * other_incoming.mValue;
		}

		if (neuron.second.mIncoming.size() > 0) {
			neuron.second.mValue = Sigmoid(sum);
		}
	}
	if (mNetwork[MAX_NODES + MOVE_LEFT].mValue > 0) {
		mGame.ExecuteAction(Dir::LEFT, Action::MOVE);
	}
	else if (mNetwork[MAX_NODES + MOVE_RIGHT].mValue > 0) {
		mGame.ExecuteAction(Dir::RIGHT, Action::MOVE);
	}
	else if (mNetwork[MAX_NODES + MOVE_DOWN].mValue > 0) {
		mGame.ExecuteAction(Dir::DOWN, Action::MOVE);
	}
	else if (mNetwork[MAX_NODES + ROTATE_LEFT].mValue > 0) {
		mGame.ExecuteAction(Dir::LEFT, Action::ROTATE);
	}
	else if (mNetwork[MAX_NODES + ROTATE_RIGHT].mValue > 0) {
		mGame.ExecuteAction(Dir::RIGHT, Action::ROTATE);
	}
	else if (mNetwork[MAX_NODES + HOLD].mValue > 0) {
		mGame.Hold();
	}
	else if (mNetwork[MAX_NODES + DROP].mValue > 0) {
		mGame.Drop();
	}
}

float Genome::GetOutputStatus(Outputs output) const {
	return mNetwork.at(MAX_NODES + static_cast<int32_t>(output)).mValue;
};

Genome Genome::CrossOver(const Genome& g1, const Genome& g2) {
	// This part contains a swap, need to check if it is needed
	assert(g1.mFitness > g2.mFitness);

	Genome child;
	std::map<int32_t, std::shared_ptr<Gene>> newInnovations;
	for (std::shared_ptr<Gene> gene : g2.mGenes) {
		newInnovations[gene->mInnovation] = gene;
	}
	for (std::shared_ptr<Gene> gene : g1.mGenes) {
		auto gene2 = newInnovations.find(gene->mInnovation);
		if (gene2 != newInnovations.end() && gene2->second->mEnabled && RandomEngine::GetRandomValue(0, 1) == 1) {
			child.mGenes.push_back(std::make_shared<Gene>(gene2->second));
		}
		else {
			child.mGenes.push_back(std::make_shared<Gene>(gene));
		}
	}
	child.mMaxNeuron = std::max(g1.mMaxNeuron, g2.mMaxNeuron);
	child.mMutationRates = g1.mMutationRates;
	return child;
}
