#include "../include/MazeCell.h"

#include <vector>

#include <random>

static int GetRandomNumber(int start, int end) {
	std::random_device rd;
	std::mt19937 gen(rd()); // Mersenne Twister random number engine
	std::uniform_real_distribution<float> dist(start, end);

	return dist(gen);
}

static int GetIndex(int r, int c, int colsCount, int rowsCount) {

	if (r < 0 || r > rowsCount-1) {
		return -1;
	}

	if (c < 0 || c > colsCount-1) {
		return -1;
	}

	return r + c * colsCount;
}

MazeCell* MazeCell::CheckNeighbours(std::vector<MazeCell>& cells, int colsCount, int rowsCount) {
	std::vector<MazeCell*> neighbours;

	int topIndex = GetIndex(i, j - 1, colsCount, rowsCount);
	if (topIndex != -1) {
		MazeCell* top = &cells[topIndex];
		if (!top->isVisited) {
			neighbours.push_back(top);
		}
	}

	int leftIndex = GetIndex(i - 1, j, colsCount, rowsCount);
	if (leftIndex != -1) {
		MazeCell* left = &cells[leftIndex];
		if (!left->isVisited) {
			neighbours.push_back(left);
		}
	}
	
	int bottomIndex = GetIndex(i, j + 1, colsCount, rowsCount);
	if (bottomIndex != -1) {
		MazeCell* bottom = &cells[bottomIndex];
		if (!bottom->isVisited) {
			neighbours.push_back(bottom);
		}
	}
	
	int rightIndex = GetIndex(i + 1, j, colsCount, rowsCount);
	if (rightIndex != -1) {
		MazeCell *right = &cells[rightIndex];
		if (!right->isVisited) {
			neighbours.push_back(right);
		}
	}
	
	if (neighbours.size() > 0) {
		int randIndex = GetRandomNumber(0, neighbours.size());
		return neighbours[randIndex];
	}

	return nullptr;
}