#pragma once

#include "game.h"
#include <GLFW/glfw3.h>



class Human {
public:
	void Update();

	bool Done();

	static void HumanKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	Game& GetGame() { return mGame; }

private:
	Game mGame;

	static bool sPressedLeft;
	static bool sPressedRight;
	static bool sPressedDown;
	static bool sPressedRotateLeft;
	static bool sPressedRotateRight;
	static bool sPressedHold;
	static bool sPressedQuit;
	static bool sPressedDrop;
	static Tetromino sRequestedTetromino;
};
