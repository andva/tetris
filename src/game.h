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

class Game {
public:
	Game();

	bool ExecuteAction(Dir d, Action a);

	void Hold();

	void Drop();

	int32_t CalculateScore(std::pair<uint32_t, bool> res, int32_t movedLines, int32_t droppedLines);

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
		int32_t mScore;
};
