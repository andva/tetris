#pragma once

#include <array>
#include <vector>
#include <cassert>
#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>

const int PIECE_GRID_SIZE = 5;
typedef std::array<std::array<int32_t, PIECE_GRID_SIZE>, PIECE_GRID_SIZE> PieceArray;
enum Direction {
	LEFT,
	RIGHT,
	DOWN
};

class PieceGrid {
	public:
		PieceGrid(int32_t x, int32_t y, PieceArray array) : mAnchor(x, y), mGrid(array) {

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
					newGrid[nx][ny] = mGrid[x][y];
				}
			}
			mGrid = newGrid;
		}

		glm::vec2 ToRenderable(int32_t x, int32_t y) {
			return glm::vec2(static_cast<float>(x) / PIECE_GRID_SIZE, static_cast<float>(y) / PIECE_GRID_SIZE);
		}

		std::vector<float> GetRenderable() {
			std::vector<float> renderable;
			for (int32_t x = 0; x < PIECE_GRID_SIZE; x++) {
				for (int32_t y = 0; y < PIECE_GRID_SIZE; y++) {
					if (mGrid[x][y] > 0) {
						glm::vec2 p1 = ToRenderable(x, y);
						glm::vec2 p2 = ToRenderable(x + 1, y);
						glm::vec2 p3 = ToRenderable(x, y + 1);
						glm::vec2 p4 = ToRenderable(x + 1, y + 1);
						renderable.push_back(p1.x); 
						renderable.push_back(p1.y);
						renderable.push_back(0);
						renderable.push_back(p3.x);
						renderable.push_back(p3.y);
						renderable.push_back(0);
						renderable.push_back(p2.x);
						renderable.push_back(p2.y);
						renderable.push_back(0);

						renderable.push_back(p2.x); 
						renderable.push_back(p2.y);
						renderable.push_back(0);
						renderable.push_back(p3.x);
						renderable.push_back(p3.y);
						renderable.push_back(0);
						renderable.push_back(p4.x);
						renderable.push_back(p4.y);
						renderable.push_back(0);
					}
				}
			}
			return renderable;
		}
	private:
		glm::ivec2 mAnchor;
		PieceArray mGrid;
};

class Grid {
	public:
		Grid() {
			
		}


	private:
		std::array<std::array<int32_t, 22>, 10> mGrid;
};
