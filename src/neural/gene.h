#pragma once

#include <memory>

struct Gene {
	Gene(std::shared_ptr<Gene> copy);
	int32_t mInto;
	int32_t mOut;
	int32_t mWeight;
	bool mEnabled;
	int32_t mInnovation;
};
