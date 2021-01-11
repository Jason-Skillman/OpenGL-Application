#pragma once
class VertexBuffer {
private:
	unsigned int rendererId;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
public:
	void Bind() const;
	void Unbind() const;
};
