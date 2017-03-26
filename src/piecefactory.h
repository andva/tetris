#pragma once

#include "piece.h"

#include <random>
#include <vector>
#include <memory>

class RandomEngine {
public:
	static int32_t GetRandomValue(int32_t low, int32_t high);
private:
	static std::random_device mDevice;
	static std::default_random_engine mGenerator;
};

class PieceFactory {
public:
	std::vector<Tetromino> GenerateSequence();
	std::shared_ptr<Piece> GetRandomPiece();

	static std::shared_ptr<Piece> CreatePiece(Tetromino t);


};