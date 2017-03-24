#pragma once

#include "piece.h"

#include <random>
#include <vector>

static std::default_random_engine generator;
static std::uniform_int_distribution<int> distribution(1, NUM_PIECES - 1);

static Piece* CreatePiece(PieceType t) {
	PieceArray pa;
	int32_t x = 3;
	int32_t y = -2;
	if (t == O) {
		pa.push_back({{1, 1}});
		pa.push_back({{1, 1}});
		x = 4;
	}
	else if (t == I) {
		pa.push_back({{0, 0, 0, 0}});
		pa.push_back({{1, 1, 1, 1}});
		pa.push_back({{0, 0, 0, 0}});
		pa.push_back({{0, 0, 0, 0}});
	}
	else if (t == T) {
		pa.push_back({{0, 1, 0}});
		pa.push_back({{1, 1, 1}});
		pa.push_back({{0, 0, 0}});
	}
	else if (t == Z) {
		pa.push_back({{1, 1, 0}});
		pa.push_back({{0, 1, 1}});
		pa.push_back({{0, 0, 0}});
	}
	else if (t == S) {
		pa.push_back({{0, 1, 1}});
		pa.push_back({{1, 1, 0}});
		pa.push_back({{0, 0, 0}});
	}
	else if (t == L) {
		pa.push_back({{0, 0, 1}});
		pa.push_back({{1, 1, 1}});
		pa.push_back({{0, 0, 0}});
	}
	else if (t == J) {
		pa.push_back({{1, 0, 0}});
		pa.push_back({{1, 1, 1}});
		pa.push_back({{0, 0, 0}});
	}
	else {
		assert(false);
	}
	return new Piece(x, y, pa, t);
}

static PieceType getRandomPiece() {
	return static_cast<PieceType>(distribution(generator));
}

static std::vector<PieceType> GenerateSequence() {
	std::vector<PieceType> pool = { J, L, O, T, I, S, Z };
	std::vector<PieceType> newSequence;
	while (pool.size() > 0) {
		std::uniform_int_distribution<int> sequenceDist(0, pool.size() - 1);
		newSequence.push_back(pool[sequenceDist(generator)]);
		auto it = std::find(pool.begin(), pool.end(), newSequence.back());
		assert(it != pool.end());
		pool.erase(it);
	}
	return newSequence;
}
