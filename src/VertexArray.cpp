#include "../include/VertexArray.h"

#include "../include/VertexBuffer.h"
#include "../include/ElementBuffer.h"

#include <glad/glad.h>

VertexArray::VertexArray(){
	glGenVertexArrays(1, &m_id);
}

void VertexArray::Bind() {
	glBindVertexArray(m_id);
}

void VertexArray::Unbind() {
	glBindVertexArray(0);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_id);
}