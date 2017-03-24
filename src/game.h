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

struct Heurestics {
	uint32_t aggregateHeight;
	uint32_t completeLines;
	uint32_t holes;
	uint32_t bumpiness;
	uint32_t score;
};

static std::unique_ptr<PieceManager> sPieceManager;
static std::shared_ptr<Grid> sGrid;
static IboData pieceIbo;
std::array<IboData, NUM_PIECES> gridIbos;
static Heurestics sHeurestics;

void CalculateScore(std::pair<uint32_t, bool> res) {
	int32_t score = 0;
	if (res.first == 0) return;
	if (!res.second) {
		if (res.first == 1) {
			score += 100;
		}
		else if (res.first == 2) {
			score += 300;
		}
		else if (res.first == 3) {
			score += 500;
		}
		else if (res.first == 4) {
			score += 800;
		}
	}
	else {
		assert(res.first <= 3);
		if (res.first == 1) {
			score += 800;
		}
		else if (res.first == 2) {
			score += 1200;
		}
		else if (res.first == 3) {
			score += 1600;
		}
	}
	sHeurestics.score += score;
}

static void print_heuristics() {
//	return;
	std::cout
		<< "AH:" << sHeurestics.aggregateHeight << " "
		<< "BN:" << sHeurestics.bumpiness << " "
		<< "CL:" << sHeurestics.completeLines << " "
		<< "HL:" << sHeurestics.holes << " "
		<< "SC:" << sHeurestics.score << " "
		<< std::endl;
}

static void update_renderable(const std::vector<uint32_t>& indices, IboData& iboData) {
	if (indices.size() == 0) {
		iboData.size = 0;
		return;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboData.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		&indices[0],
		GL_STATIC_DRAW);
	iboData.size = static_cast<int>(indices.size());
}

static bool ExecuteAction(Direction d, Action a) {
	std::function<void(Direction)> action;
	if (a == MOVE) {
		action = [] (Direction d) { sPieceManager->GetPiece()->Move(d); };
	}
	else if (a == ROTATE) {
		action = [] (Direction d) {sPieceManager->GetPiece()->Rotate(d); };
	}
	action(d);
	if (!sGrid->Validate(sPieceManager->GetPiece()->GetCollisionObject())) {
		action(GetOpposite(d));
		return false;
	}
	else {
		update_renderable(sPieceManager->GetPiece()->GetRenderable(), pieceIbo);
		return true;
	}
}

static void Hold() {
	if (sPieceManager->Hold()) {
		update_renderable(sPieceManager->GetPiece()->GetRenderable(), pieceIbo);
	}
}

static void Drop(std::shared_ptr<Grid> grid) {
	while (ExecuteAction(DOWN, MOVE));
	auto collObj = sPieceManager->GetPiece()->GetCollisionObject();
	if (sGrid->IsAnythingUnder(collObj)) {
		auto res = grid->Place(
								collObj,
								sPieceManager->GetPiece()->GetType());
		sHeurestics.completeLines += res.first;
		
		sPieceManager->SetNext();
		
		update_renderable(sPieceManager->GetPiece()->GetRenderable(), pieceIbo);
		for (int i = 0; i < gridIbos.size(); i++) {
			update_renderable(sGrid->GetRenderable(i + 1), gridIbos[i]);
		}
		sGrid->CalculateGridHeuristics(
									   sHeurestics.holes,
									   sHeurestics.aggregateHeight,
									   sHeurestics.bumpiness
									   );
		CalculateScore(res);
		print_heuristics();
	}
}

class Game {
public:
	Game() {
		
	}
	

	private:
		
};
