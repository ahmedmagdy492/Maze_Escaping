
#include "../include/Texture.h"
#include "../include/stb_image.h"

#include <glad/glad.h>


Texture::Texture(const std::string& imagePath, unsigned int textureUnit, unsigned int imgPixelFormat) : m_textureUnit(textureUnit) {
	int width = 0, height = 0, nChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &nChannels, 0);

	if (!data)
		throw new std::string("Unable to load provided image");

	glGenTextures(1, &m_id);
	glActiveTexture(m_textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imgPixelFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_id);
}