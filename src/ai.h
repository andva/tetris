#pragma once

#include "grid.h"
#include "game.h"
#include "piecemanager.h"
#include "piecefactory.h"

#include <memory>

struct AiWeights {
	float height;
	float lines;
	float holes;
	float bumpiness;
};

struct Heurestics {
	Heurestics() : aggregateHeight(0), completeLines(0), holes(0), bumpiness(0), score(0) {};
	uint32_t aggregateHeight;
	uint32_t completeLines;
	uint32_t holes;
	uint32_t bumpiness;
	uint32_t score;
};

class Ai {
	public:
		Ai(float he, float li, float ho, float bu);
		Game& GetGame() {
			return mGame;
		}
		void CalculateOptimal();
	private:
		AiWeights mWeights;
		Game mGame;
		PieceManager mPieceManager;
};
