#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) : rendererId(0) {
	GLCall(glGenBuffers(1, &rendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int usage) : rendererId(0) {
	GLCall(glGenBuffers(1, &rendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &rendererId))
}

void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}

void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::BufferSubData(const void* data, unsigned int size) const {
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}
