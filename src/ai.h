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

class Ai {
	public:
		Ai(float he, float li, float ho, float bu) {
			mWeights.height = he;
			mWeights.lines = li;
			mWeights.holes = ho;
			mWeights.bumpiness = bu;
		}
		Piece* CalculateOptimal(PieceType t, std::shared_ptr<Grid> grid) {
			std::shared_ptr<Piece> optimalP;
			
			float optimal = -999999999.0;
			int32_t pcStore = 0;
			PieceType ht = sPieceManager->HasHoldType() ? sPieceManager->GetHoldType() : sPieceManager->GetNextType();
			for (int pc = 0; pc < 2; pc++) {
				for (int i = 0; i < 4; i++) {
					for (int x = 0; x < 9; x++) {
						std::shared_ptr<Piece> p = CreatePiece(pc == 0 ? ht : t);
						sPieceManager->ReplaceCurrentPiece(p);
						for (int r = 0; r < i; ++r) {
							ExecuteAction(LEFT, ROTATE);
						}
						for (int m = 0; m < 4; m++) {
							ExecuteAction(LEFT, MOVE);
						}
						for (int m = 0; m < x; m++) {
							ExecuteAction(RIGHT, MOVE);
						}
 					std::shared_ptr<Grid> gCopy(new Grid(*grid.get()));
						//					Drop(gCopy);
						while (ExecuteAction(DOWN, MOVE));
						auto res = gCopy->Place(p->GetCollisionObject(), p->GetType());
						Heurestics h;
						gCopy->CalculateGridHeuristics(h.holes, h.aggregateHeight, h.bumpiness);
						h.completeLines += res.first;
						float c =
						static_cast<float>(h.aggregateHeight) * mWeights.height +
						static_cast<float>(h.completeLines) * mWeights.lines +
						static_cast<float>(h.bumpiness) * mWeights.bumpiness +
						static_cast<float>(h.holes) * mWeights.holes;
						if (optimal < c) {
							optimal = c;
							optimalP = p;
							pcStore = pc;
						}
					}
				}
			}
			if (pcStore == 0) {
				Hold();
			}
			sPieceManager->ReplaceCurrentPiece(optimalP);
		}
	private:
		AiWeights mWeights;
};
