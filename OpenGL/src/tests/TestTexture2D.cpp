#include "TestTexture2D.h"

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

	TestTexture2D::TestTexture2D()
		: translationA(300, 300, 0), translationB(200, 200, 0),
		projection(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f)),
		view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))) {
		
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,   //Bottom left
			50.0f, -50.0f, 1.0f, 0.0f,    //Bottom right
			50.0f, 50.0f, 1.0f, 1.0f,     //Top right
			-50.0f, 50.0f, 0.0f, 1.0f     //Top left
		};
		unsigned int indices[]{
			0, 1, 2,
			2, 3, 0
		};

        
        //----- Vertex buffer -----
        int segments = 4, segSize = 4;
        //VertexBuffer vb(positions, segments * segSize * sizeof(float));
        vertexBuffer = std::make_unique<VertexBuffer>(positions, segments * segSize * sizeof(float));

        indexBuffer = std::make_unique<IndexBuffer>(indices, 6);
        
        

        //----- Vertex array -----
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
		
        //Add the buffer and layout to the vertex array
        vertexArray = std::make_unique<VertexArray>();
        vertexArray->AddBuffer(*vertexBuffer, layout);

		

        //----- Shader -----
        shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        shader->Bind();
        shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		
        
        //----- Texture -----
        //Texture texture("res/textures/logo_google.png");
        texture = std::make_unique<Texture>("res/textures/logo_google.png");
        texture->Bind();
        
        //Value: the texture slot to use
        shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D() {}

	void TestTexture2D::OnUpdate(float deltaTime) {}

	void TestTexture2D::OnRender() {
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

        //Object 2
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = projection * view * model;

            shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*vertexArray, *indexBuffer, *shader);
        }
	}

	void TestTexture2D::OnImGuiRender() {
        ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 800.0f);
        ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 800.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}
