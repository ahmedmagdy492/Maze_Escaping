#pragma once

#include "MazeCell.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Player.h"

#include <vector>

class MazeRenderer {
private:
	std::vector<MazeCell> cells;
	const unsigned char cellSize = 5; // in pixels
	unsigned short colsCount;
	unsigned short rowsCount;
	Texture* wallTexture;
	VertexArray* va;
	VertexBuffer* vb;
	ElementBuffer* eb;
	Shader* curShader;
	Player* player;
	int width, height;

public:
	MazeRenderer(int width, int height);

	void Update(glm::vec3 cameraFront, bool keys[], float deltaTime);
	void RenderMaze();

	void Draw3dLine(float x, float y, int w, int h, float angle);

	~MazeRenderer();
};