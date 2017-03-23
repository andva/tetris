#pragma once

enum Direction {
	LEFT,
	RIGHT,
	DOWN,
	UP
};

enum Action {
	MOVE,
	ROTATE,
	DROP,
	PLACE
};

static Direction GetOpposite(Direction d) {
	if (d == LEFT) {
		return RIGHT;
	}
	if (d == RIGHT) {
		return LEFT;
	}
	if (d == UP) {
		return DOWN;
	}
	if (d == DOWN) {
		return UP;
	}
}