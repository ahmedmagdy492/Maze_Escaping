
#include "../include/ElementBuffer.h"

#include <glad/glad.h>

ElementBuffer::ElementBuffer(unsigned int data[], int size) {
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void ElementBuffer::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBuffer::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

ElementBuffer::~ElementBuffer() {
	glDeleteBuffers(1, &m_id);
}