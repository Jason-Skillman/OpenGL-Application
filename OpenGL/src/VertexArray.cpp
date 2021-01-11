#include "VertexArray.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &rendererId));
	GLCall(glBindVertexArray(rendererId));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &rendererId));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind();
	vb.Bind();

	unsigned int offset = 0;

	const auto& elements = layout.GetElement();
	for(unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(0, element.count, element.type, element.normalized, layout.GetStride(), reinterpret_cast<const void*>(offset)));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(rendererId));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}
