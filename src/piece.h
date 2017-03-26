#pragma once

#include "direction.h"
#include "tetromino.h"

#include <array>
#include <vector>
#include <cassert>
#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>

typedef std::vector<std::vector<int32_t>> PieceArray;

class Piece {
	public:
		Piece(int32_t x, int32_t y, PieceArray box, Tetromino type);
	
		PieceArray CreateArray();
	
		void Rotate(Dir d);

		void Move(Dir d);

		glm::vec2 ToRenderable(int32_t x, int32_t y);

		const std::vector<glm::ivec2>& GetCollisionObject() const;

		Tetromino GetType() const;
	
	private:
		void UpdateCollisionObject();

		std::vector<glm::ivec2> mCollisionObject;
		glm::ivec2 mAnchor;
		PieceArray mGrid;
		Tetromino mType;
};
