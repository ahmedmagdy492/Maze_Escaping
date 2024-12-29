#pragma once

#include <string>

class Texture {
private:
	unsigned int m_id;

public:
	Texture(const std::string& imagePath, int textureUnit, unsigned int imgPixelFormat);

	void Bind(int textureUnit);
	void Unbind();

	~Texture();
};