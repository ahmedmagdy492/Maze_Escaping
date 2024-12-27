#pragma once

#include <string>

class Texture {
private:
	unsigned int m_id;
	unsigned int m_textureUnit;

public:
	Texture(const std::string& imagePath, unsigned int textureUnit, unsigned int imgPixelFormat);

	void Bind();
	void Unbind();

	~Texture();
};