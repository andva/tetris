#pragma once

#include "direction.h"
#include "tetromino.h"

#include <array>
#include <vector>
#include <set>
#include <utility>
#include <glm/vec2.hpp>
#include <numeric>

const int32_t BOARD_GRID_SIZE_X = 10;
const int32_t BOARD_GRID_SIZE_Y = 20;

class Grid {
	public:
		Grid();
	
		void ResetGrid();
	
		bool Validate(std::vector<glm::ivec2> collisionObj) const;
	
		bool IsAnythingUnder(const std::vector<glm::ivec2>& collisionObj) const;

		std::pair<uint32_t, bool> Place(const std::vector<glm::ivec2>& collisionObj, int32_t color);
	
		std::vector<unsigned int> GetRenderable(int32_t color);
	
		std::vector<unsigned int> GetRenderable(const std::vector<glm::ivec2> collisionObj);

		bool HasLost() const;
	
		void CalculateGridHeuristics(uint32_t& holes, uint32_t& aggregateHeight, uint32_t& bumpiness);
	
	private:
		std::array<std::array<int32_t, BOARD_GRID_SIZE_X>, BOARD_GRID_SIZE_Y> mGrid;
		bool mLost;
};
