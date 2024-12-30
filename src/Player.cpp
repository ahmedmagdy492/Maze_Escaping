
#include "../include/Player.h"

#include <iostream>

#include "../include/AudioPlayer.h"

#define KEY_W 87
#define KEY_S 83
#define KEY_D 68
#define KEY_A 65


Player::Player(glm::vec3 initPos) : position(initPos) {
	std::string filePath = "resources/assets/walk.mp3";
	audioPlayer.LoadAudio(filePath);
}

glm::mat4 Player::Move(bool keys[], float deltaTime, glm::vec3 direction) {
	float speed = 8.0f * deltaTime;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	if (keys[KEY_W])
	{
		if (!audioPlayer.IsPlaying() || audioPlayer.HasPlaybackEnded()) {
			audioPlayer.PlayAudio();
		}
		position += speed * direction;
	}
	else {
		if (audioPlayer.IsPlaying()) {
			audioPlayer.StopAudio();
		}
	}

	return glm::lookAt(position, position + direction, cameraUp);
}

Player::~Player() {
	audioPlayer.UnloadAudio();
}