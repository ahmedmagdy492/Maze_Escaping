
#include "../include/Player.h"

#include <iostream>

#define KEY_W 87
#define KEY_S 83
#define KEY_D 68
#define KEY_A 65


Player::Player(glm::vec3 initPos) : position(initPos) {
}

glm::mat4 Player::Move(bool keys[], float deltaTime, glm::vec3 velocity) {
	float speed = 10.0f * deltaTime;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	if (keys[KEY_W])
		position += speed * velocity;
	if (keys[KEY_D])
		position += glm::normalize(glm::cross(velocity, cameraUp)) * speed;
	if (keys[KEY_A])
		position -= glm::normalize(glm::cross(velocity, cameraUp)) * speed;

	return glm::lookAt(position, position + velocity, cameraUp);
}

Player::~Player() {
}