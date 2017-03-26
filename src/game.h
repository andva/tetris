#pragma once

#include "grid.h"
#include "piecemanager.h"
#include "piece.h"

#include <memory>
#include <array>
#include <functional>

struct IboData {
	uint32_t id;
	int32_t size;
};

//struct Heurestics {
//	uint32_t aggregateHeight;
//	uint32_t completeLines;
//	uint32_t holes;
//	uint32_t bumpiness;
//	uint32_t score;
//};


//static void print_heuristics() {
//	return;
//	std::cout
//		<< "AH:" << sHeurestics.aggregateHeight << " "
//		<< "BN:" << sHeurestics.bumpiness << " "
//		<< "CL:" << sHeurestics.completeLines << " "
//		<< "HL:" << sHeurestics.holes << " "
//		<< "SC:" << sHeurestics.score << " "
//		<< std::endl;
//}



static void Hold() {
	
}


class Game {
public:
	Game();

	bool ExecuteAction(Dir d, Action a);

	void Hold();

	void Drop();

	int32_t CalculateScore(std::pair<uint32_t, bool> res);

	void Update();

	const Grid& GetGrid() const {
		return mGrid;
	}

	const PieceManager& GetPieceManager() const {
		return mPieceManager;
	}

	void Render(int32_t loc) const;

	void Reset();

	private:
		PieceManager mPieceManager;
		Grid mGrid;
		IboData mPieceIbo;
		std::array<IboData, static_cast<size_t>(Tetromino::NUM_PIECES)> mGridIbos;
};
