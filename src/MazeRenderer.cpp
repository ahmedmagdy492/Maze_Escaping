
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

#include <stack>

// data section
//static float wallVertexData[] = {
//	-0.5f, 0.5f, 0.0f,   0.0f, 0.0f,
//	 0.5f, 0.5f, 0.0f,   1.0f, 0.0f,
//	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
//	 0.5f, -0.5f, 0.0f,  1.0f, 1.0f
//};

static float wallVertexData[] = {
	// Front face
   -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,  // Top-left
	0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  // Top-right
   -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,  // Bottom-left
	0.5f, -0.5f,  0.5f,   1.0f, 1.0f,  // Bottom-right

	// Back face
	-0.5f,  0.5f, -0.5f,   0.0f, 0.0f,  // Top-left
	 0.5f,  0.5f, -0.5f,   1.0f, 0.0f,  // Top-right
	-0.5f, -0.5f, -0.5f,   0.0f, 1.0f,  // Bottom-left
	 0.5f, -0.5f, -0.5f,   1.0f, 1.0f,  // Bottom-right

	 // Left face
	 -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,  // Top-left
	 -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  // Top-right
	 -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,  // Bottom-left
	 -0.5f, -0.5f,  0.5f,   1.0f, 1.0f,  // Bottom-right

	 // Right face
	  0.5f,  0.5f, -0.5f,   0.0f, 0.0f,  // Top-left
	  0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  // Top-right
	  0.5f, -0.5f, -0.5f,   0.0f, 1.0f,  // Bottom-left
	  0.5f, -0.5f,  0.5f,   1.0f, 1.0f,  // Bottom-right

	  // Top face
	  -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,  // Top-left
	   0.5f,  0.5f, -0.5f,   1.0f, 0.0f,  // Top-right
	  -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  // Bottom-left
	   0.5f,  0.5f,  0.5f,   1.0f, 1.0f,  // Bottom-right

	   // Bottom face
	   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  // Top-left
		0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  // Top-right
	   -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,  // Bottom-left
		0.5f, -0.5f,  0.5f,   1.0f, 1.0f   // Bottom-right
};

static unsigned int elementBufferData[] = {
		0, 1, 2,  1, 3, 2,  // Front face
		4, 5, 6,  5, 7, 6,  // Back face
		8, 9, 10, 9, 11, 10,  // Left face
		12, 13, 14, 13, 15, 14,  // Right face
		16, 17, 18, 17, 19, 18,  // Top face
		20, 21, 22, 21, 23, 22
};

static void RemoveWalls(MazeCell* current, MazeCell* next) {
	static int count = 0;

	if ((current->i - next->i) == -1) {
		current->walls.willDrawRight = false;
		next->walls.willDrawLeft = false;
		count++;
	}
	else if ((current->i - next->i) == 1) {
		current->walls.willDrawLeft = false;
		next->walls.willDrawRight = false;
		count++;
	}
	else if ((current->j - next->j) == -1) {
		current->walls.willDrawBottom = false;
		next->walls.willDrawTop = false;
		count++;
	}
	else if ((current->j - next->j) == 1) {
		current->walls.willDrawTop = false;
		next->walls.willDrawBottom = false;
		count++;
	}
	std::cout << "Count of walls removed: " << count / 2 << std::endl;
}

static bool AreThereAnyLeftUnVisitedCell(std::vector<MazeCell>& cells) {
	for (MazeCell& cell : cells) {
		if (!cell.isVisited) {
			return true;
		}
	}

	return false;
}

MazeRenderer::MazeRenderer(int width, int height) : width(width), height(height), colsCount(0), rowsCount(0) {
	wallTexture = new Texture("resources/assets/bg.png", GL_TEXTURE0, GL_RGBA);
	landTexture = new Texture("resources/assets/land.png", GL_TEXTURE0, GL_RGBA);
	wall2Texture = new Texture("resources/assets/bg2.png", GL_TEXTURE0, GL_RGBA);
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
			mazeCell.i = j;
			mazeCell.j = i;

			mazeCell.walls.willDrawTop = true;
			mazeCell.walls.willDrawBottom = true;
			mazeCell.walls.willDrawLeft = true;
			mazeCell.walls.willDrawRight = true;
			
			cells.push_back(mazeCell);
		}
	}

	// generating the maze
	MazeCell* current = &cells[0];
	current->isVisited = true;
	std::stack<MazeCell*> cellsStack;

	while (AreThereAnyLeftUnVisitedCell(cells)) {
		MazeCell* next = current->CheckNeighbours(cells, colsCount, rowsCount);
		if (next) {
			next->isVisited = true;
			cellsStack.push(current);
			RemoveWalls(current, next);

			current = next;
		}
		else if(!cellsStack.empty()) {
			current = cellsStack.top();
			cellsStack.pop();
		}
	}
}

void MazeRenderer::Update(glm::vec3 cameraFront, bool keys[], float deltaTime) {
	va->Bind();
	curShader->Use();

	std::string viewName = "un_view";
	glm::mat4 viewMat = player->Move(keys, deltaTime, cameraFront);
	curShader->SetUniformMat4(viewName, glm::value_ptr(viewMat));
}

void MazeRenderer::Draw3dLine(float x, float y, int w, int h, float angle) {
	glm::mat4 modelMat = glm::mat4(1.0f);
	glm::vec3 translateVec = glm::vec3(x - (w / 2.0f), 0.0f, y * -1 - (h / 2.0f));
	modelMat = glm::translate(modelMat, translateVec);

	glm::vec3 rotationVec = glm::vec3(0.0f, 1.0f, 0.0f);
	modelMat = glm::rotate(modelMat, glm::radians(angle), rotationVec);

	glm::vec3 scaleVec = glm::vec3(w, h, 1.0);
	modelMat = glm::scale(modelMat, scaleVec);

	std::string name = "un_model";
	curShader->SetUniformMat4(name, glm::value_ptr(modelMat));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
}

void MazeRenderer::DrawPlane(float x, float y, int w, int h) {
	glm::mat4 modelMat = glm::mat4(1.0f);

	glm::vec3 translateVec = glm::vec3(x - (w / 2.0f), -5.0f, y * -1 - (w / 2.0f));
	modelMat = glm::translate(modelMat, translateVec);

	glm::vec3 rotationVec = glm::vec3(1.0f, 0.0f, 0.0f);
	modelMat = glm::rotate(modelMat, glm::radians(90.0f), rotationVec);

	glm::vec3 scaleVec = glm::vec3(w, h, 0.0f);
	modelMat = glm::scale(modelMat, scaleVec);

	std::string name = "un_model";
	curShader->SetUniformMat4(name, glm::value_ptr(modelMat));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void MazeRenderer::RenderMaze() {
	// do the maze rendering here
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective<float>(glm::radians(90.0f), width / height, 0.1f, 100.0f);
	std::string projectionName = "un_projection";
	curShader->SetUniformMat4(projectionName, glm::value_ptr(projection));
	
	landTexture->Bind(GL_TEXTURE0);

	DrawPlane(cells[cells.size()-1].x, (cellSize / 2), cellSize * 10, cellSize * 10);

	for (int i = 0; i < cells.size(); ++i) {
		if (cells[i].walls.willDrawTop) {
			wallTexture->Bind(GL_TEXTURE0);
			Draw3dLine(cells[i].x, cells[i].y, cellSize, cellSize, 0.0f);
		}
		if (cells[i].walls.willDrawLeft) {
			wall2Texture->Bind(GL_TEXTURE0);
			Draw3dLine(cells[i].x - (cellSize / 2), cells[i].y + (cellSize / 2), cellSize, cellSize, 90.0f);
		}
		if (cells[i].walls.willDrawBottom) {
			wallTexture->Bind(GL_TEXTURE0);
			Draw3dLine(cells[i].x, cells[i].y + (cellSize / 2), cellSize, cellSize, 0.0f);
		}
		if (cells[i].walls.willDrawRight) {
			wall2Texture->Bind(GL_TEXTURE0);
			Draw3dLine(cells[i].x + (cellSize / 2), cells[i].y + (cellSize / 2), cellSize, cellSize, 90.0f);
		}
	}
}

MazeRenderer::~MazeRenderer() {
	delete wallTexture;
	delete landTexture;
	delete wall2Texture;
	delete curShader;
	delete vb;
	delete eb;
	delete va;
	delete player;
}