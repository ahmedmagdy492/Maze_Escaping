
#include "../include/Texture.h"
#include "../include/stb_image.h"

#include <glad/glad.h>

#include <iostream>


Texture::Texture(const std::string& imagePath, int textureUnit, unsigned int imgPixelFormat) {
	int width = 0, height = 0, nChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &nChannels, 0);

	if (!data)
		throw new std::string("Unable to load provided image");

	if (imgPixelFormat != GL_RGB && imgPixelFormat != GL_RGBA) {
		stbi_image_free(data);
		throw std::runtime_error("Unsupported image format");
	}

	glGenTextures(1, &m_id);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, imgPixelFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

void Texture::Bind(int textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_id);
}