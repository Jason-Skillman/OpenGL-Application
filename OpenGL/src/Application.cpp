#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	//V-sync
    glfwSwapInterval(1);

    //Init GLEW
	//Must be init after a valid context has been created (window)
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: glewInit() failed to initialize" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;


    {
        //----- Position and indices -----
        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f,   //Bottom left
            200.0f, 100.0f, 1.0f, 0.0f,    //Bottom right
            200.0f, 200.0f, 1.0f, 1.0f,     //Top right
            100.0f, 200.0f, 0.0f, 1.0f     //Top left
        };
        //float positions[] = {
        //    -0.5f, -0.5f,
        //    0.5f, -0.5f,
        //    0.5f, 0.5f,
        //    -0.5f, 0.5f
        //};
        unsigned int indices[]{
            0, 1, 2,
            2, 3, 0
        };


    	//Enable texture blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    	

        //----- VertexArray/Buffer -----
        VertexArray va;

        int segments = 4, segSize = 4;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    	
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        //std::cout << "Size: " << layout.GetElementSize() << std::endl;

    	//Add the buffer and layout to the vertex array
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);



    	//Create a projection matrix
        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    	
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));



    	
        //----- Shader -----
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        

    	//Create a texture
        Texture texture("res/textures/logo_google.png");
        texture.Bind();

    	//Value: the texture slot to use
        shader.SetUniform1i("u_Texture", 0);


        //Clear
        va.Unbind();
        shader.Unbind();
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));



        //----- Renderer -----
        Renderer renderer;



    	//----- Setup ImGUI -----
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));


        
        glm::vec3 translation(300, 300, 0);

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while(!glfwWindowShouldClose(window)) {
            /* Render here */
            renderer.Clear();

            //New Frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            //Create a model view projection matrix(MVP)
            glm::mat4 mvp = projection * view * model;

        	
            shader.Bind();
            //shader.SetUniform4f("u_Color", 0.4f, 0.3f, 0.8f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);

        	
            renderer.Draw(va, ib, shader);


        	//Animation
            /*if(r > 1.0f) increment = -0.5f;
            if(r < 1.0f) increment = 0.5f;
            r += increment;*/

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("ImGUI");
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 800.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            //Render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    //Shutdown
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
