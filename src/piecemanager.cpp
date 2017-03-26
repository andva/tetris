#include "piecemanager.h"

PieceManager::PieceManager() 
	: mHoldLock(false)
	, mStoredPiece(Tetromino::NUM_PIECES)
{
	FillPieceQueue();
	SetNext();
}

bool PieceManager::Hold()
{
	if (mStoredPiece == Tetromino::NUM_PIECES) {
		mStoredPiece = mPiece->GetType();
		SetNext();
		mHoldLock = true;
		return true;
	}
	else if (!mHoldLock) {
		Tetromino newType = mPiece->GetType();
		mPiece = mFactory.CreatePiece(mStoredPiece);
		mStoredPiece = newType;
		mHoldLock = true;
		return true;
	}
	return false;
}

void PieceManager::SetNext()
{
	mPiece = mFactory.CreatePiece(mPieceQueue.front());
	mPieceQueue.pop_front();
	FillPieceQueue();
	mHoldLock = false;
}

void PieceManager::FillPieceQueue()
{
	if (mPieceQueue.size() < 7) {
		auto newSet = mFactory.GenerateSequence();
		for (auto piece : newSet) {
			mPieceQueue.push_back(piece);
		}
	}
}

void PieceManager::ReplaceCurrentPiece(std::shared_ptr<Piece> p)
{
	mPiece = p;
}

bool PieceManager::IsPieceStored() const
{
	return mHoldLock;
}

const std::shared_ptr<Piece> PieceManager::GetPiece() const
{
	return mPiece;
}

bool PieceManager::HasHoldType() const
{
	return mStoredPiece != Tetromino::NUM_PIECES;
}

Tetromino PieceManager::GetHoldType() const
{
	return mStoredPiece;
}

Tetromino PieceManager::GetNextType() const
{
	return mPieceQueue.front();
}

std::string PieceManager::GetFutureState() const
{
	std::stringstream ss;
	ss << ToName(mPieceQueue.front()) << " ";
	ss << "(" << ToName(mStoredPiece) << ")";
	return ss.str();
}

