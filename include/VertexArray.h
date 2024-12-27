#pragma once

#include "VertexBuffer.h"
#include "ElementBuffer.h"

class VertexArray {
private:
	unsigned int m_id;

public:
	VertexArray();
	void Bind();
	void Unbind();
	~VertexArray();
};