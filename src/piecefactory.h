#pragma once

#include "piece.h"

#include <random>

enum PieceType {
	O = 1,
	I = 2,
	T = 3,
	S = 4,
	Z = 5,
	L = 6,
	NUM_PIECES,
};

static std::default_random_engine generator;
static std::uniform_int_distribution<int> distribution(1,6);

static Piece* CreatePiece(PieceType t) {
	PieceArray pa;
	
	if (t == O) {
		pa.push_back({{1, 1}});
		pa.push_back({{1, 1}});
	}
	else if (t == I) {
		pa.push_back({{0, 1, 0, 0}});
		pa.push_back({{0, 1, 0, 0}});
		pa.push_back({{0, 1, 0, 0}});
		pa.push_back({{0, 1, 0, 0}});
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
	return new Piece(0, 0, pa, t + 1);
}

static Piece* CreatePiece() {
	return CreatePiece(static_cast<PieceType>(distribution(generator)));
}
