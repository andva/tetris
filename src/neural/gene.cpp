#include "gene.h"

Gene::Gene(std::shared_ptr<Gene> copy) {
	mInto = copy->mInto;
	mOut = copy->mOut;
	mWeight = copy->mWeight;
	mEnabled = copy->mEnabled;
	mInnovation = copy->mInnovation;
}
