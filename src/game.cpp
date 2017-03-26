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
{
	glGenBuffers(1, &mPieceIbo.id);
	for (int i = 0; i < mGridIbos.size(); i++) {
		glGenBuffers(1, &mGridIbos[i].id);
	}
}

bool Game::ExecuteAction(Dir d, Action a)
{
	if (a == Action::DROP) {
		Drop();
		return true;
	}
	if (a == Action::HOLD) {
		Hold();
		return true;
	}
	std::function<void(Dir)> action;
	if (a == Action::MOVE) {
		action = [&] (Dir d) { mPieceManager.GetPiece()->Move(d); };
	}
	else if (a == Action::ROTATE) {
		action = [&] (Dir d) {mPieceManager.GetPiece()->Rotate(d); };
	}
	action(d);
	auto collObj = mPieceManager.GetPiece()->GetCollisionObject();
	if (!mGrid.Validate(collObj)) {
		action(GetOpposite(d));
		return false;
	}
	else {
		UpdateRenderable(mGrid.GetRenderable(collObj), mPieceIbo);
		return true;
	}
}

void Game::Hold()
{
	if (mPieceManager.Hold()) {
		UpdateRenderable(mGrid.GetRenderable(mPieceManager.GetPiece()->GetCollisionObject()), mPieceIbo);
	}
}

void Game::Drop()
{
	while (ExecuteAction(Dir::DOWN, Action::MOVE));
	auto collObj = mPieceManager.GetPiece()->GetCollisionObject();
	if (mGrid.IsAnythingUnder(collObj)) {
		auto res = mGrid.Place(
			collObj,
			1);
			//static_cast<int32_t>(mPieceManager.GetPiece()->GetType()));
		//sHeurestics.completeLines += res.first;

		mPieceManager.SetNext();

		UpdateRenderable(mGrid.GetRenderable(mPieceManager.GetPiece()->GetCollisionObject()), mPieceIbo);
		for (int i = 0; i < mGridIbos.size(); i++) {
			UpdateRenderable(mGrid.GetRenderable(i + 1), mGridIbos[i]);
		}
		//mGrid->CalculateGridHeuristics(
			//sHeurestics.holes,
			//sHeurestics.aggregateHeight,
			//sHeurestics.bumpiness
			//);
		CalculateScore(res);
		//print_heuristics();
	}
}

int32_t Game::CalculateScore(std::pair<uint32_t, bool> res)
{
	int32_t score = 0;
	if (res.first == 0) return score;
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
}
