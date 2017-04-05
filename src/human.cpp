#include "human.h"

void Human::HumanKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
		sPressedRotateLeft = true;
	}
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		sPressedRotateRight = true;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
		sPressedLeft = true;
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
		sPressedRight = true;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
		sPressedDown = true;
	}
	else if (key == GLFW_KEY_H && action == GLFW_RELEASE) {
		sPressedHold = true;
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
		sPressedDrop = true;
	}
	else if (key == GLFW_KEY_I && action == GLFW_RELEASE) {
		sRequestedTetromino = Tetromino::I;
	}
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		sRequestedTetromino = Tetromino::S;
	}
	else if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
		sRequestedTetromino = Tetromino::Z;
	}
	else if (key == GLFW_KEY_O && action == GLFW_RELEASE) {
		sRequestedTetromino = Tetromino::O;
	}
	else if (key == GLFW_KEY_J && action == GLFW_RELEASE) {
		sRequestedTetromino = Tetromino::J;
	}
	else if (key == GLFW_KEY_L && action == GLFW_RELEASE) {
		sRequestedTetromino = Tetromino::L;
	}
}

bool Toggle(bool& b) {
	if (b) {
		b = false;
		return true;
	}
	return false;
}

void Human::Update()
{
	if (Toggle(sPressedLeft)) {
		mGame.ExecuteAction(Dir::LEFT, Action::MOVE);
	}
	if (Toggle(sPressedRight)) {
		mGame.ExecuteAction(Dir::RIGHT, Action::MOVE);
	}
	if (Toggle(sPressedDown)) {
		mGame.ExecuteAction(Dir::DOWN, Action::MOVE);
	}
	if (Toggle(sPressedDrop)) {
		mGame.Drop();
	}
	if (Toggle(sPressedHold)) {
		mGame.Hold();
	}
	if (Toggle(sPressedRotateLeft)) {
		mGame.ExecuteAction(Dir::LEFT, Action::ROTATE);
	}
	if (Toggle(sPressedRotateRight)) {
		mGame.ExecuteAction(Dir::RIGHT, Action::ROTATE);
	}
	if (sRequestedTetromino != Tetromino::NUM_PIECES) {
		mGame.SwitchTetromino(sRequestedTetromino);
		sRequestedTetromino = Tetromino::NUM_PIECES;
	}
}

bool Human::Done()
{
	if (sPressedQuit == true) {
		sPressedQuit = false;
		return true;
	}
	return false;
}

bool Human::sPressedLeft = false;

bool Human::sPressedDrop = false;

bool Human::sPressedQuit = false;

bool Human::sPressedHold = false;

bool Human::sPressedRotateRight = false;

bool Human::sPressedRotateLeft = false;

bool Human::sPressedDown = false;

bool Human::sPressedRight = false;

Tetromino Human::sRequestedTetromino = Tetromino::NUM_PIECES;
