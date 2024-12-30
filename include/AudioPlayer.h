#pragma once

#include "miniaudio.h"

#include <string>

namespace AudioLib {

	class AudioPlayer {
		private:
			ma_sound sound;
			ma_engine engine;
			bool isPlaying = false;

		public:
			void LoadAudio(std::string& filePath);
			void PlayAudio();
			bool HasPlaybackEnded();
			void StopAudio();
			void UnloadAudio();

			inline bool IsPlaying() {
				return isPlaying;
			}
	};
}