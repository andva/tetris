#pragma once

#include "piece.h"

#include <random>

static std::default_random_engine generator;
static std::uniform_int_distribution<int> distribution(1, NUM_PIECES - 1);

static Piece* CreatePiece(PieceType t) {
	PieceArray pa;
	
	if (t == O) {
		pa.push_back({{1, 1}});
		pa.push_back({{1, 1}});
	}
	else if (t == I) {
		pa.push_back({{0, 0, 1, 0}});
		pa.push_back({{0, 0, 1, 0}});
		pa.push_back({{0, 0, 1, 0}});
		pa.push_back({{0, 0, 1, 0}});
	}
	else if (t == T) {
		pa.push_back({{1, 1, 1}});
		pa.push_back({{0, 1, 0}});
		pa.push_back({{0, 0, 0}});
	}
	else if (t == S) {
		pa.push_back({{0, 0, 1}});
		pa.push_back({{0, 1, 1}});
		pa.push_back({{0, 1, 0}});
	}
	else if (t == Z) {
		pa.push_back({{1, 0, 0}});
		pa.push_back({{1, 1, 0}});
		pa.push_back({{0, 1, 0}});
	}
	else if (t == L) {
		pa.push_back({{1, 0, 0}});
		pa.push_back({{1, 0, 0}});
		pa.push_back({{1, 1, 0}});
	}
	else if (t == J) {
		pa.push_back({{0, 1, 0}});
		pa.push_back({{0, 1, 0}});
		pa.push_back({{1, 1, 0}});
	}
	else {
		assert(false);
	}
	return new Piece(0, 0, pa, t);
}

static Piece* CreatePiece() {
	return CreatePiece(static_cast<PieceType>(distribution(generator)));
}
