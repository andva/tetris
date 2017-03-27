#pragma once

#include "gene.h"

#include <vector>
#include <memory>

struct Neuron {
	std::vector<std::shared_ptr<Gene>> mIncoming;
	float mValue;
};
