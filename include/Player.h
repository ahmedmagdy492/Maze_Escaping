#pragma once

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

/// <summary>
/// the player is the camera no distinction between them in this game
/// </summary>
class Player {
private:
	glm::vec3 position;

public:
	Player(glm::vec3 initPos);

	glm::mat4 Move(bool keys[], float deltaTime, glm::vec3 velocity);

	~Player();
};