#pragma once

#include <vector>


struct MazeWall {
	bool willDrawTop = false, willDrawLeft = false, willDrawBottom = false, willDrawRight = false;
};

struct MazeCell {
	float x, y;
	bool isVisited = false;
	MazeWall walls;

	int i, j;

	MazeCell* CheckNeighbours(std::vector<MazeCell>& cells, int colsCount, int rowsCount);
};