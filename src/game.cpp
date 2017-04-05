#include "game.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace {
void UpdateRenderable(const std::vector<uint32_t>& indices, IboData& iboData) {
	if (indices.size() == 0) {
		iboData.size = 0;
		return;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboData.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		&indices[0],
		GL_STATIC_DRAW);
	iboData.size = static_cast<int>(indices.size());
}
}

Game::Game()
	: mScore(0)
{
	glGenBuffers(1, &mPieceIbo.id);
	for (int i = 0; i < mGridIbos.size(); i++) {
		glGenBuffers(1, &mGridIbos[i].id);
	}
}

void Game::SwitchTetromino(Tetromino t) {
	mPieceManager.ReplaceCurrentPiece(PieceFactory::CreatePiece(t));
	auto collObj = mPieceManager.GetPiece()->GetCollisionObject();
	UpdateRenderable(mGrid.GetRenderable(collObj), mPieceIbo);
}

bool Game::Rotate(Dir d) {
	mPieceManager.GetPiece()->Rotate(d);
	auto collObj = mPieceManager.GetPiece()->GetCollisionObject();
	if (!mGrid.Validate(collObj)) {
		// Try offsetting in different directions
		if (d == Dir::LEFT) {
			if (ExecuteAction(Dir::LEFT, Action::MOVE)) {
				return true;
			}
			if (ExecuteAction(Dir::RIGHT, Action::MOVE)) {
				return true;
			}
			if (ExecuteAction(Dir::DOWN, Action::MOVE)) {
				return true;
			}
			if (ExecuteAction(Dir::DOWN_LEFT, Action::MOVE)) {
				return true;
			}
			if (ExecuteAction(Dir::DOWN_RIGHT, Action::MOVE)) {
				return true;
			}
		}
		else if (d == Dir::RIGHT) {
			if (ExecuteAction(Dir::RIGHT, Action::MOVE)) {
				return true;
			}
			if (ExecuteAction(Dir::LEFT, Action::MOVE)) {
				return true;
			}
			if (ExecuteAction(Dir::DOWN, Action::MOVE)) {
				return true;
			}
			if (ExecuteAction(Dir::DOWN_RIGHT, Action::MOVE)) {
				return true;
			}
			if (ExecuteAction(Dir::DOWN_LEFT, Action::MOVE)) {
				return true;
			}
		}
		else { assert(false); }
		return false;
	}
	return true;
}

bool Game::Move(Dir d) {
	mPieceManager.GetPiece()->Move(d);

	auto collObj = mPieceManager.GetPiece()->GetCollisionObject();
	if (!mGrid.Validate(collObj)) {
		mPieceManager.GetPiece()->Move(GetOpposite(d));
		return false;
	}
	return true;
}

bool Game::ExecuteAction(Dir d, Action a)
{
	bool updated = false;
	if (a == Action::DROP) {
		updated = Drop();
	}
	if (a == Action::HOLD) {
		updated = Hold();
	}
	if (a == Action::MOVE) {
		updated = Move(d);
	}
	if (a == Action::ROTATE) {
		updated = Rotate(d);
	}
	if (updated) {
		auto collObj = mPieceManager.GetPiece()->GetCollisionObject();
		UpdateRenderable(mGrid.GetRenderable(collObj), mPieceIbo);
		return true;
	}
	return false;
}

bool Game::Hold()
{
	return mPieceManager.Hold();
}

bool Game::Drop()
{
	int32_t preDropMovedLines = mPieceManager.GetPiece()->GetMovedLines();
	while (ExecuteAction(Dir::DOWN, Action::MOVE));
	auto collObj = mPieceManager.GetPiece()->GetCollisionObject();
	if (mGrid.IsAnythingUnder(collObj)) {
		int32_t movedLines = mPieceManager.GetPiece()->GetMovedLines();
		int32_t droppedLines = movedLines - preDropMovedLines;
		auto res = mGrid.Place(
			collObj,
			1);
		mPieceManager.SetNext();
		auto collObj = mPieceManager.GetPiece()->GetCollisionObject();
		UpdateRenderable(mGrid.GetRenderable(collObj), mPieceIbo);
		for (int i = 0; i < mGridIbos.size(); i++) {
			UpdateRenderable(mGrid.GetRenderable(i + 1), mGridIbos[i]);
		}
		mScore += CalculateScore(res, movedLines, droppedLines);
		return true;
	}
	return false;
}

int32_t Game::CalculateScore(std::pair<uint32_t, bool> res, int32_t movedLines, int32_t droppedLines)
{
	assert(movedLines >= 0);
	int32_t score = movedLines + droppedLines;
	if (res.first > 0) {
		if (!res.second) {
			if (res.first == 1) {
				score += 100;
			}
			else if (res.first == 2) {
				score += 300;
			}
			else if (res.first == 3) {
				score += 500;
			}
			else if (res.first == 4) {
				score += 800;
			}
		}
		else {
			assert(res.first <= 3);
			if (res.first == 1) {
				score += 800;
			}
			else if (res.first == 2) {
				score += 1200;
			}
			else if (res.first == 3) {
				score += 1600;
			}
		}
	}
	return score;
}

void Game::Update()
{

}

void draw(IboData data, GLint colorLoc, int32_t color) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.id);
	if (colorLoc != -1) {
		glUniform1ui(colorLoc, color);
	}
	glDrawElements(
		GL_TRIANGLES,
		data.size,
		GL_UNSIGNED_INT,
		(void*) 0
		);
}

void Game::Render(int32_t loc) const
{
	for (uint32_t i = 0; i < mGridIbos.size(); i++) {
		draw(mGridIbos[i], loc, i + 1);
	}

	draw(mPieceIbo, loc, static_cast<int32_t>(mPieceManager.GetPiece()->GetType()));
}

void Game::Reset()
{
	mGrid = Grid();
	UpdateRenderable(mGrid.GetRenderable(mPieceManager.GetPiece()->GetCollisionObject()), mPieceIbo);
	for (int i = 0; i < mGridIbos.size(); i++) {
		UpdateRenderable(mGrid.GetRenderable(i + 1), mGridIbos[i]);
	}
	mScore = 0;
}
