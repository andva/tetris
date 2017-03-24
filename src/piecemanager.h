#pragma once

#include "piece.h"
#include "piecefactory.h"

#include <list>
#include <memory>
#include <sstream>

class PieceManager {
public:
	PieceManager()
		: mPiece(CreatePiece(getRandomPiece()))
		, mHoldLock(false)
		, mStoredPiece(NUM_PIECES)
	{
		FillPieceQueue();
	}

	bool Hold() {
		if (mStoredPiece == NUM_PIECES) {
			mStoredPiece = mPiece->GetType();
			SetNext();
			mHoldLock = true;
			return true;
		}
		else if (!mHoldLock) {
			PieceType newType = mPiece->GetType();
			mPiece.reset(CreatePiece(mStoredPiece));
			mStoredPiece = newType;
			mHoldLock = true;
			return true;
		}
		return false;
	}

	void SetNext() {
		mPiece.reset(CreatePiece(mPieceQueue.front()));
		mPieceQueue.pop_front();
		FillPieceQueue();
		mHoldLock = false;
	}

	void FillPieceQueue() {
		while (mPieceQueue.size() < 4) {
			mPieceQueue.push_back(getRandomPiece());
		}
	}

	bool IsPieceStored() const {
		return mHoldLock;
	}

	const std::shared_ptr<Piece> GetPiece() const {
		return mPiece;
	}

	std::string GetFutureState() const {
		std::stringstream ss;
		for (auto pt : mPieceQueue) {
			ss << ToName(pt) << " ";
		}
		ss << "(" << ToName(mStoredPiece) << ")";
		return ss.str();
	}

private:
	std::list<PieceType> mPieceQueue;
	PieceType mStoredPiece;
	std::shared_ptr<Piece> mPiece;
	bool mHoldLock;

};