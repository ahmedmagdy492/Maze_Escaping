
#include "../include/MazeRenderer.h"

#include "../include/MazeCell.h"
#include "../include/Texture.h"
#include "../include/VertexArray.h"
#include "../include/VertexBuffer.h"
#include "../include/ElementBuffer.h"
#include "../include/Shader.h"
#include "../include/Player.h"

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

// data section
static float wallVertexData[] = {
	-0.5f, 0.5f, 0.0f,   0.0f, 0.0f,
	 0.5f, 0.5f, 0.0f,   1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,  1.0f, 1.0f
};

static unsigned int elementBufferData[] = {
		0, 2, 3,
		0, 1, 3
};

MazeRenderer::MazeRenderer(int width, int height) : width(width), height(height), colsCount(0), rowsCount(0) {
	wallTexture = new Texture("resources/assets/bg.png", GL_TEXTURE0, GL_RGBA);
	curShader = new Shader("resources/shaders/vertex.shader", "resources/shaders/fragment.shader");
	va = new VertexArray();
	va->Bind();
	vb = new VertexBuffer(wallVertexData, sizeof(wallVertexData));
	eb = new ElementBuffer(elementBufferData, sizeof(elementBufferData));
	player = new Player(glm::vec3(0.0f, 0.0f, -0.3f));

	// generating the maze
	colsCount = 10;
	rowsCount = 10;

	for (int i = 0; i < rowsCount; ++i) {
		for (int j = 0; j < colsCount; ++j) {
			MazeCell mazeCell;
			mazeCell.x = cellSize * j;
			mazeCell.y = cellSize * i;

			mazeCell.walls.willDrawTop = true;
			mazeCell.walls.willDrawBottom = true;
			mazeCell.walls.willDrawLeft = true;
			mazeCell.walls.willDrawRight = true;
			
			cells.push_back(mazeCell);
		}
	}
}

void MazeRenderer::Update(glm::vec3 cameraFront, bool keys[], float deltaTime) {
	curShader->Use();
	va->Bind();

	std::string viewName = "un_view";
	glm::mat4 viewMat = player->Move(keys, deltaTime, cameraFront);
	curShader->SetUniformMat4(viewName, glm::value_ptr(viewMat));
}

void MazeRenderer::Draw3dLine(float x, float y, int w, int h, float angle) {
	glm::mat4 modelMat = glm::mat4(1.0f);
	glm::vec3 translateVec = glm::vec3(x, 0.0f, y * -1.0f);
	modelMat = glm::translate(modelMat, translateVec);

	glm::vec3 rotationVec = glm::vec3(0.0f, 1.0f, 0.0f);
	modelMat = glm::rotate(modelMat, glm::radians(angle), rotationVec);

	glm::vec3 scaleVec = glm::vec3(cellSize, cellSize, 0.0f);
	modelMat = glm::scale(modelMat, scaleVec);

	std::string name = "un_model";
	curShader->SetUniformMat4(name, glm::value_ptr(modelMat));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void MazeRenderer::RenderMaze() {
	// do the maze rendering here
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective<float>(glm::radians(45.0f), width / height, 0.1f, 100.0f);
	std::string projectionName = "un_projection";
	curShader->SetUniformMat4(projectionName, glm::value_ptr(projection));

	for (int i = 0; i < cells.size(); ++i) {
		if (cells[i].walls.willDrawTop) {
			Draw3dLine(cells[i].x, cells[i].y, cellSize, cellSize, -90.0f);
		}
		if (cells[i].walls.willDrawLeft) {
			Draw3dLine(cells[i].x, cells[i].y, cellSize, cellSize, 0.0f);
		}
	}
}

MazeRenderer::~MazeRenderer() {
	delete wallTexture;
	delete curShader;
	delete vb;
	delete eb;
	delete va;
	delete player;
}