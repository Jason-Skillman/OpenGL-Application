#include "TestBatchRenderColors.h"

#include <GL/glew.h>

#include "imgui/imgui.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"

namespace test {

    TestBatchRenderColors::TestBatchRenderColors()
		: translationA(300, 300, 0), translationB(200, 200, 0),
		projection(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f)),
		view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))) {
		
		float vertices[] = {
			-50.0f, -50.0f,     0.0f, 0.0f,    0.18f, 0.6f, 0.96f, 1.0f,      //Bottom left
            50.0f, -50.0f,      1.0f, 0.0f,    0.18f, 0.6f, 0.96f, 1.0f,      //Bottom right
            50.0f, 50.0f,       1.0f, 1.0f,    0.18f, 0.6f, 0.96f, 1.0f,      //Top right
            -50.0f, 50.0f,      0.0f, 1.0f,    0.18f, 0.6f, 0.96f, 1.0f,      //Top left

            150.0f, -50.0f,     0.0f, 0.0f,    1.0f, 0.93f, 0.24f, 1.0f,
            250.0f, -50.0f,     1.0f, 0.0f,    1.0f, 0.93f, 0.24f, 1.0f,
            250.0f, 50.0f,      1.0f, 1.0f,    1.0f, 0.93f, 0.24f, 1.0f,
            150.0f, 50.0f,      0.0f, 1.0f,    1.0f, 0.93f, 0.24f, 1.0f,
		};
		unsigned int indices[]{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

        
        //----- Vertex buffer -----
        int segments = 8, segSize = 8;  //y, x
        vertexBuffer = std::make_unique<VertexBuffer>(vertices, segments * segSize * sizeof(float));

        indexBuffer = std::make_unique<IndexBuffer>(indices, 12);
        
        

        //----- Vertex array -----
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        layout.Push<float>(4);
		
        //Add the buffer and layout to the vertex array
        vertexArray = std::make_unique<VertexArray>();
        vertexArray->AddBuffer(*vertexBuffer, layout);

		

        //----- Shader -----
        shader = std::make_unique<Shader>("res/shaders/VertexColors.shader");
        shader->Bind();
        //shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);



        //----- Texture -----
        texture = std::make_unique<Texture>("res/textures/logo_google.png");
        //texture->Bind();

        //Value: the texture slot to use
        //shader->SetUniform1i("u_Texture", 0);
	}

    TestBatchRenderColors::~TestBatchRenderColors() {}

	void TestBatchRenderColors::OnUpdate(float deltaTime) {}

	void TestBatchRenderColors::OnRender() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        texture->Bind();

        //Create a projection matrix
        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        //Object 1
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = projection * view * model;

            shader->Bind();
            shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*vertexArray, *indexBuffer, *shader);
        }
	}

	void TestBatchRenderColors::OnImGuiRender() {
        ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 800.0f);
    }

}
