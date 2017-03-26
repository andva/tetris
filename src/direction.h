#pragma once

#include <cassert>

enum class Dir {
	LEFT,
	RIGHT,
	DOWN,
	UP
};

enum class Action {
	MOVE,
	ROTATE,
	DROP,
	HOLD,
	PLACE
};

static Dir GetOpposite(Dir d) {
	if (d == Dir::LEFT) {
		return Dir::RIGHT;
	}
	if (d == Dir::RIGHT) {
		return Dir::LEFT;
	}
	if (d == Dir::UP) {
		return Dir::DOWN;
	}
	if (d == Dir::DOWN) {
		return Dir::UP;
	}
	else {
		assert(false);
		return Dir::DOWN;
	}
}