#pragma once

#include "direction.h"

#include <array>
#include <vector>
#include <set>
#include <glm/vec2.hpp>

const int BOARD_GRID_SIZE_X = 10;
const int BOARD_GRID_SIZE_Y = 20;

class Grid {
	public:
		Grid() : mLost(false) {
			ResetGrid();
		}
	
		void ResetGrid() {
			for (uint32_t y = 0; y < BOARD_GRID_SIZE_Y; y++) {
				for (uint32_t x = 0; x < BOARD_GRID_SIZE_X; x++) {
					mGrid[y][x] = 0;
				}
			}
			mLost = false;
		}
	
		bool Validate(std::vector<glm::ivec2> collisionObj) const {
			assert(collisionObj.size() > 0);
			for (glm::ivec2 c : collisionObj) {
				if (c.x < 0 || c.y < -2
					|| c.x >= BOARD_GRID_SIZE_X
					|| c.y >= BOARD_GRID_SIZE_Y) {
					return false;
				}
				if (c.y >= 0) {
					if (mGrid[c.y][c.x] > 0) {
						return false;
					}
				}
			}
			return true;
		}
	
		bool IsAnythingUnder(const std::vector<glm::ivec2>& collisionObj) const {
			for (glm::ivec2 c : collisionObj) {
				if (c.y == BOARD_GRID_SIZE_Y - 1) {
					return true;
				}
				else if (mGrid[c.y + 1][c.x] > 0) {
					return true;
				}
			}
			return false;
		}

		bool Place(const std::vector<glm::ivec2>& collisionObj, int32_t color) {
			assert(Validate(collisionObj));
			if (IsAnythingUnder(collisionObj)) {
				std::set<int32_t> rows;
				for (glm::ivec2 c : collisionObj) {
					if (c.y < 0) {
						mLost = true;
						return true;
					}
					mGrid[c.y][c.x] = color;
					rows.insert(c.y);
				}
				for (uint32_t r : rows) {
					bool complete = true;
					for (uint32_t x = 0; x < BOARD_GRID_SIZE_X; x++) {
						if (mGrid[r][x] == 0) {
							complete = false;
						}
					}
					if (complete) {
						for (uint32_t x = 0; x < BOARD_GRID_SIZE_X; x++) {
							mGrid[r][x] = -1;
						}
					}
				}
				for (int32_t y = BOARD_GRID_SIZE_Y - 1; y >= 0; y--) {
					for (int32_t x = 0; x < BOARD_GRID_SIZE_X; x++) {
						if (mGrid[y][x] < 0) {
							uint32_t cy = y - 1;
							while (mGrid[cy][x] < 0 && cy > 0) {
								cy--;
							}
							if (cy == 0) {
								mGrid[y][x] = 0;
							}
							else {
								mGrid[y][x] = mGrid[cy][x];
								mGrid[cy][x] = -1;
							}
						}
					}
				}
				return true;
			}
			return false;
		}
	
		std::vector<unsigned int> GetRenderable(int32_t color) {
			assert(color != 0);
			std::vector<unsigned int> renderable;
			
			for (uint32_t y = 0; y < BOARD_GRID_SIZE_Y; y++) {
				for (uint32_t x = 0; x < BOARD_GRID_SIZE_X; x++) {
					if (mGrid[y][x] == color) {
						renderable.push_back(x + y * (BOARD_GRID_SIZE_X + 1));
						renderable.push_back(x + 1 + y * (BOARD_GRID_SIZE_X + 1));
						renderable.push_back(x + 1 + (y + 1) * (BOARD_GRID_SIZE_X + 1));
						
						renderable.push_back(x + y * (BOARD_GRID_SIZE_X + 1));
						renderable.push_back(x + (y + 1) * (BOARD_GRID_SIZE_X + 1));
						renderable.push_back((x + 1) + (y + 1) * (BOARD_GRID_SIZE_X + 1));
					}
				}
			}
			return renderable;
		}
	
		bool HasLost() const {
			return mLost;
		}
	private:
		std::array<std::array<int32_t, BOARD_GRID_SIZE_X>, BOARD_GRID_SIZE_Y> mGrid;
		bool mLost;
};
