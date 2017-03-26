#pragma once

#include <string>

enum class Tetromino : int32_t {
	O = 1,
	I = 2,
	T = 3,
	S = 4,
	Z = 5,
	L = 6,
	J = 7,
	NUM_PIECES,
};

static std::string ToName(Tetromino t) {
	switch (t) {
	case Tetromino::O:
		return "O";
	case Tetromino::I:
		return "I";
	case Tetromino::T:
		return "T";
	case Tetromino::S:
		return "S";
	case Tetromino::Z:
		return "Z";
	case Tetromino::L:
		return "L";
	case Tetromino::J:
		return "J";
	default:
		return "";
	}
}