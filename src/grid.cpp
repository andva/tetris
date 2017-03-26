#include "grid.h"

Grid::Grid() : mLost(false)
{
	ResetGrid();
}

void Grid::ResetGrid()
{
	for (uint32_t y = 0; y < BOARD_GRID_SIZE_Y; y++) {
		for (uint32_t x = 0; x < BOARD_GRID_SIZE_X; x++) {
			mGrid[y][x] = 0;
		}
	}
	mLost = false;
}

bool Grid::Validate(std::vector<glm::ivec2> collisionObj) const
{
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

bool Grid::IsAnythingUnder(const std::vector<glm::ivec2>& collisionObj) const
{
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

std::pair<uint32_t, bool> Grid::Place(const std::vector<glm::ivec2>& collisionObj, int32_t color)
{
	if (!Validate(collisionObj)) {
		mLost = true;
	}
	if (mLost) return std::make_pair(0, false);

	std::pair<uint32_t, bool> result = { 0, false };
	// Place piece in grid, check if lost
	for (glm::ivec2 c : collisionObj) {
		if (c.y < 0) {
			mLost = true;
			return result;
		}
		// Check for T-Spin
		if (c.y > 0) {
			int32_t v = mGrid[c.y - 1][c.x];
			bool tspin = (v > 0);
			if (tspin) {
				result.second = result.second || tspin;
			}
		}
	}
	std::set<int32_t> rows;
	for (glm::ivec2 c : collisionObj) {
		mGrid[c.y][c.x] = color;
		rows.insert(c.y);
	}
	// Go trough the rows that we affected, check if any lines were cleared
	for (uint32_t r : rows) {
		bool complete = true;
		for (uint32_t x = 0; x < BOARD_GRID_SIZE_X; x++) {
			if (mGrid[r][x] == 0) {
				complete = false;
			}
		}
		if (complete) {
			result.first++;
			for (uint32_t x = 0; x < BOARD_GRID_SIZE_X; x++) {
				mGrid[r][x] = -1;
			}
		}
	}
	// Collapse completed lines
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
	return result;
}

std::vector<unsigned int> Grid::GetRenderable(int32_t color)
{
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

std::vector<unsigned int> Grid::GetRenderable(const std::vector<glm::ivec2> collisionObj)
{
	std::vector<unsigned int> renderable;
	for (glm::ivec2 c : collisionObj) {
		if (c.y >= 0) {
			renderable.push_back(c.x + c.y * (BOARD_GRID_SIZE_X + 1));
			renderable.push_back(c.x + 1 + c.y * (BOARD_GRID_SIZE_X + 1));
			renderable.push_back(c.x + 1 + (c.y + 1) * (BOARD_GRID_SIZE_X + 1));

			renderable.push_back(c.x + c.y * (BOARD_GRID_SIZE_X + 1));
			renderable.push_back(c.x + (c.y + 1) * (BOARD_GRID_SIZE_X + 1));
			renderable.push_back((c.x + 1) + (c.y + 1) * (BOARD_GRID_SIZE_X + 1));
		}
	}
	return renderable;
}

bool Grid::HasLost() const
{
	return mLost;
}

void Grid::CalculateGridHeuristics(uint32_t& holes, uint32_t& aggregateHeight, uint32_t& bumpiness)
{
	std::vector<uint32_t> h = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (uint32_t y = 0; y < BOARD_GRID_SIZE_Y; y++) {
		for (uint32_t x = 0; x < BOARD_GRID_SIZE_X; x++) {
			if (mGrid[y][x] > 0) {
				h[x] = std::max(h[x], BOARD_GRID_SIZE_Y - y);
			}
		}
	}
	aggregateHeight = std::accumulate(h.begin(), h.end(), 0u);
	bumpiness = 0;
	for (int i = 0; i < h.size() - 1; i++) {
		bumpiness += abs(static_cast<int32_t>(h[i] - h[i + 1]));
	}

	holes = 0;
	for (int32_t y = BOARD_GRID_SIZE_Y - 1; y >= 0; y--) {
		for (uint32_t x = 0; x < BOARD_GRID_SIZE_X; x++) {
			if (mGrid[y][x] == 0 && BOARD_GRID_SIZE_Y - y < h[x]) {
				holes++;
			}
		}
	}
}

