#pragma once

#include "piece.h"

#include <random>
#include <vector>
#include <memory>

class PieceFactory {
public:
	PieceFactory();
	std::vector<Tetromino> GenerateSequence();
	std::shared_ptr<Piece> GetRandomPiece();

	static std::shared_ptr<Piece> CreatePiece(Tetromino t);

private:
	std::random_device mDevice;
	std::default_random_engine mGenerator;
};