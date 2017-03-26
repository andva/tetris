#include "piecefactory.h"

PieceFactory::PieceFactory()
	: mGenerator(mDevice()) { }

std::shared_ptr<Piece> PieceFactory::CreatePiece(Tetromino t) {
	PieceArray pa;
	int32_t x = 3;
	int32_t y = 0;
	if (t == Tetromino::O) {
		pa.push_back({ { 1, 1 } });
		pa.push_back({ { 1, 1 } });
		x = 4;
	}
	else if (t == Tetromino::I) {
		pa.push_back({ { 0, 0, 0, 0 } });
		pa.push_back({ { 1, 1, 1, 1 } });
		pa.push_back({ { 0, 0, 0, 0 } });
		pa.push_back({ { 0, 0, 0, 0 } });
		y = -1;
	}
	else if (t == Tetromino::T) {
		pa.push_back({ { 0, 1, 0 } });
		pa.push_back({ { 1, 1, 1 } });
		pa.push_back({ { 0, 0, 0 } });
	}
	else if (t == Tetromino::Z) {
		pa.push_back({ { 1, 1, 0 } });
		pa.push_back({ { 0, 1, 1 } });
		pa.push_back({ { 0, 0, 0 } });
	}
	else if (t == Tetromino::S) {
		pa.push_back({ { 0, 1, 1 } });
		pa.push_back({ { 1, 1, 0 } });
		pa.push_back({ { 0, 0, 0 } });
	}
	else if (t == Tetromino::L) {
		pa.push_back({ { 0, 0, 1 } });
		pa.push_back({ { 1, 1, 1 } });
		pa.push_back({ { 0, 0, 0 } });
	}
	else if (t == Tetromino::J) {
		pa.push_back({ { 1, 0, 0 } });
		pa.push_back({ { 1, 1, 1 } });
		pa.push_back({ { 0, 0, 0 } });
	}
	else {
		assert(false);
	}
	return std::make_shared<Piece>(x, y, pa, t);
}

std::vector<Tetromino> PieceFactory::GenerateSequence() {
	std::vector<Tetromino> pool = { 
		Tetromino::J,
		Tetromino::L,
		Tetromino::O,
		Tetromino::T,
		Tetromino::I,
		Tetromino::S,
		Tetromino::Z
	};
	std::vector<Tetromino> newSequence;
	while (pool.size() > 0) {
		std::uniform_int_distribution<uint32_t> sequenceDist(0, static_cast<uint32_t>(pool.size()) - 1);
		newSequence.push_back(pool[sequenceDist(mGenerator)]);
		auto it = std::find(pool.begin(), pool.end(), newSequence.back());
		assert(it != pool.end());
		pool.erase(it);
	}
	return newSequence;
}

std::shared_ptr<Piece> PieceFactory::GetRandomPiece()
{
	std::uniform_int_distribution<int> distribution(0, static_cast<int32_t>(Tetromino::NUM_PIECES) - 1);
	return CreatePiece(static_cast<Tetromino>(distribution(mGenerator)));
}
