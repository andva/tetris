#pragma once

#include <vector>

#include <array>
#include <cassert>
#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>

typedef std::array<std::array<int32_t, PIECE_GRID_SIZE>, PIECE_GRID_SIZE> PieceArray;

class Piece {
	public:
		Piece(int32_t x, int32_t y, PieceArray array) : mAnchor(x, y), mGrid(array) {

		}
		void Rotate(Direction d) {
			PieceArray newGrid;
			for (int32_t y = 0; y < PIECE_GRID_SIZE; y++) {
				for (int32_t x = 0; x < PIECE_GRID_SIZE; x++) {
					glm::vec2 p = glm::vec2(static_cast<float>(x), static_cast<float>(y));
					glm::mat2 rot = d == RIGHT ? glm::mat2(0, 1, -1, 0) : glm::mat2(0, -1, 1, 0);
					
					int xOffset = d == LEFT ? PIECE_GRID_SIZE - 1 : 0;
					int yOffset = d == LEFT ? 0 : PIECE_GRID_SIZE - 1;
					glm::vec2 newP = p * rot;
					int32_t nx = static_cast<int32_t>(newP.x) + xOffset;
					int32_t ny = static_cast<int32_t>(newP.y) + yOffset;
					assert(nx >= 0 && ny >= 0);
					newGrid[ny][nx] = mGrid[y][x];
				}
			}
			mGrid = newGrid;
		}

		void Move(Direction d) {
			if (d == LEFT) {
				mAnchor += glm::ivec2(-1, 0);
			}
			else if (d == RIGHT) {
				mAnchor += glm::ivec2(1, 0);
			}
			else if (d == DOWN) {
				mAnchor += glm::ivec2(0, 1);
			}
			else if (d == UP) {
				mAnchor += glm::ivec2(0, -1);
			}
		}

		glm::vec2 ToRenderable(int32_t x, int32_t y) {
			return glm::vec2(static_cast<float>(x) / PIECE_GRID_SIZE, static_cast<float>(y) / PIECE_GRID_SIZE);
		}

		uint32_t GridToRenderId(uint32_t x, uint32_t y) const {
			return x + mAnchor.x + (y + mAnchor.y) * (BOARD_GRID_SIZE_X + 1);
		}

		std::vector<glm::ivec2> GetCollisionObject() const {
			std::vector<glm::ivec2> collisionObject;
			for (uint32_t x = 0; x < PIECE_GRID_SIZE; x++) {
				for (uint32_t y = 0; y < PIECE_GRID_SIZE; y++) {
					if (mGrid[y][x] > 0) {
						collisionObject.push_back(glm::ivec2(x + mAnchor.x, y + mAnchor.y));
					}
				}
			}
			return collisionObject;
		}

		std::vector<unsigned int> GetRenderable() {
			std::vector<unsigned int> renderable;
			
			for (uint32_t x = 0; x < PIECE_GRID_SIZE; x++) {
				for (uint32_t y = 0; y < PIECE_GRID_SIZE; y++) {
					if (mGrid[y][x] > 0) {
 						renderable.push_back(GridToRenderId(x, y));
						renderable.push_back(GridToRenderId(x + 1, y + 1));
						renderable.push_back(GridToRenderId(x + 1, y));
						
						renderable.push_back(GridToRenderId(x, y));
						renderable.push_back(GridToRenderId(x + 1, y + 1));
						renderable.push_back(GridToRenderId(x, y + 1));
					}
				}
			}
			return renderable;
		}
	private:
		glm::ivec2 mAnchor;
		PieceArray mGrid;
};
