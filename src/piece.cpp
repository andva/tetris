#include "piece.h"

Piece::Piece(int32_t x, int32_t y, PieceArray box, Tetromino type)
	: mAnchor(x, y)
	, mGrid(box)
	, mType(type)
	, mMovedLines(0)
{
	for (int y = 0; y < box.size(); y++) {
		assert(box.size() == box[y].size());
	}
	UpdateCollisionObject();
}

PieceArray Piece::CreateArray()
{
	PieceArray pa;
	for (int32_t y = 0; y < mGrid.size(); y++) {
		std::vector<int32_t> row;
		for (int32_t x = 0; x < mGrid[y].size(); x++) {
			row.push_back(0);
		}
		pa.push_back(row);
	}
	return pa;
}

void Piece::Rotate(Dir d)
{
	PieceArray newGrid = CreateArray();
	for (int32_t y = 0; y < mGrid.size(); y++) {
		for (int32_t x = 0; x < mGrid[y].size(); x++) {
			glm::vec2 p = glm::vec2(static_cast<float>(x), static_cast<float>(y));
			glm::mat2 rot = d == Dir::RIGHT ? glm::mat2(0, 1, -1, 0) : glm::mat2(0, -1, 1, 0);

			int xOffset = d == Dir::LEFT ? static_cast<int>(mGrid.size()) - 1 : 0;
			int yOffset = d == Dir::LEFT ? 0 : static_cast<int>(mGrid.size()) - 1;
			glm::vec2 newP = p * rot;
			int32_t nx = static_cast<int32_t>(newP.x) + xOffset;
			int32_t ny = static_cast<int32_t>(newP.y) + yOffset;
			assert(nx >= 0 && ny >= 0);
			newGrid[ny][nx] = mGrid[y][x];
		}
	}
	mGrid = newGrid;
	UpdateCollisionObject();
}

void Piece::Move(Dir d)
{
	if (d == Dir::LEFT) {
		mAnchor += glm::ivec2(-1, 0);
	}
	else if (d == Dir::RIGHT) {
		mAnchor += glm::ivec2(1, 0);
	}
	else if (d == Dir::DOWN) {
		mAnchor += glm::ivec2(0, 1);
		mMovedLines++;
	}
	else if (d == Dir::UP) {
		mAnchor += glm::ivec2(0, -1);
		mMovedLines--;
	}
	UpdateCollisionObject();
}

glm::vec2 Piece::ToRenderable(int32_t x, int32_t y)
{
	return glm::vec2(static_cast<float>(x) / mGrid.size(), static_cast<float>(y) / mGrid.size());
}

const std::vector<glm::ivec2>& Piece::GetCollisionObject() const
{
	return mCollisionObject;
}

Tetromino Piece::GetType() const
{
	return mType;
}

void Piece::UpdateCollisionObject()
{
	mCollisionObject.clear();
	for (uint32_t x = 0; x < mGrid.size(); x++) {
		for (uint32_t y = 0; y < mGrid.size(); y++) {
			if (mGrid[y][x] > 0) {
				mCollisionObject.push_back(glm::ivec2(x + mAnchor.x, y + mAnchor.y));
			}
		}
	}
}

