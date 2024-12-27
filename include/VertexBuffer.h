#pragma once

#pragma once

class VertexBuffer {
private:
	unsigned int m_id;

public:
	/// <summary>
	/// for now i am going to assume that the data buffer is always going to contain
	/// 2 vertex attributes (vertices 3 floats, texture coordinates 2 floats)</summary>
	/// <param name="data"></param>
	/// <param name="size"></param>
	VertexBuffer(float data[], int size);

	void Bind();
	void Unbind();

	~VertexBuffer();
};