#pragma once

class ElementBuffer {
private:
	unsigned int m_id;

public:
	ElementBuffer(unsigned int data[], int size);

	void Bind();
	void Unbind();

	~ElementBuffer();
};