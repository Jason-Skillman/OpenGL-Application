#pragma once

class VertexBuffer {
private:
	unsigned int rendererId;
	
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const void* data, unsigned int size, unsigned int usage);
	~VertexBuffer();
	
public:
	void Bind() const;
	void Unbind() const;
	void BufferSubData(const void* data, unsigned int size) const;
	
};
