#pragma once

#include "piece.h"
#include "piecefactory.h"

#include <list>
#include <memory>
#include <sstream>

class PieceManager {
public:
	PieceManager();

	bool Hold();

	void SetNext();

	void FillPieceQueue();

	void ReplaceCurrentPiece(std::shared_ptr<Piece> p);

	bool IsPieceStored() const;

	const std::shared_ptr<Piece> GetPiece() const;

	bool HasHoldType() const;

	Tetromino GetHoldType() const;
	Tetromino GetNextType() const;
	std::string GetFutureState() const;

private:
	PieceFactory mFactory;
	std::list<Tetromino> mPieceQueue;
	Tetromino mStoredPiece;
	std::shared_ptr<Piece> mPiece;
	bool mHoldLock;

};
