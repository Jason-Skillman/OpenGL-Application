#include "Renderer.h"

#include <iostream>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

void GLClearErrors() {
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while(GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] " << error <<
            " " << function <<
            " " << file <<
            ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    //count: number of indices to draw
	//type: should be same as index buffer
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
