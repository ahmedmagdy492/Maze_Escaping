#pragma once


namespace MazeEscaping {

	struct Color {
		float red;
		float green;
		float blue;
		float alpha;
	};

	const char* window_title = "Maze Escaping";
	bool fullscreen_mode = false;
	int screen_width = 1048;
	int screen_height = 768;
	Color background_color = { 0.0f, 0.0f, 0.0f, 1.0f };
	
}