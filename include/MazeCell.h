#pragma once


struct MazeWall {
	bool willDrawTop = false, willDrawLeft = false, willDrawBottom = false, willDrawRight = false;
};

struct MazeCell {
	float x, y;
	bool isVisited = false;
	MazeWall walls;
};