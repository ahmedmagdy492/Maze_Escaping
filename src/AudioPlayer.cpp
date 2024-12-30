

#include "../include/AudioPlayer.h"

#include <iostream>
#include <sstream>

namespace AudioLib {
	static std::string PrintStringError(std::string msg, ma_result code) {
		std::stringstream stream;
		stream << msg;
		stream << ma_result_description(code);
		return stream.str();
	}

	void AudioPlayer::LoadAudio(std::string& filePath) {
		ma_result result;

		result = ma_engine_init(nullptr, &engine);
		if (result != MA_SUCCESS) {
			throw std::runtime_error(PrintStringError("Failed to init engine: ", result));
		}

		result = ma_sound_init_from_file(&engine, filePath.c_str(), 0, nullptr, nullptr, &sound);
		if (result != MA_SUCCESS) {
			throw std::runtime_error(PrintStringError("Failed to init sound: ", result));
		}
	}

	bool AudioPlayer::HasPlaybackEnded() {
		return ma_sound_at_end(&sound);
	}

	void AudioPlayer::PlayAudio() {
		ma_result result = ma_sound_seek_to_pcm_frame(&sound, 0);
		if (result != MA_SUCCESS) {
			throw std::runtime_error(PrintStringError("Failed to reset playback position: ", result));
		}

		result = ma_sound_start(&sound);
		isPlaying = true;
	}

	void AudioPlayer::StopAudio() {
		if (!isPlaying) return;

		ma_result result = ma_sound_stop(&sound);
		isPlaying = false;
	}

	void AudioPlayer::UnloadAudio() {
		ma_sound_uninit(&sound);
		ma_engine_uninit(&engine);
	}
}