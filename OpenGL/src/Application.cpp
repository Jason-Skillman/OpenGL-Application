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

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestBatchRender.h"
#include "tests/TestBatchRenderColors.h"
#include "tests/TestBatchRenderTextures.h"
#include "tests/TestBatchRenderDynamic.h"

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
    	//Enable texture blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    	
        Renderer renderer;


        //----- Setup ImGUI -----
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));


        test::Test* currentTest;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

    	//Register all of the tests
        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
        testMenu->RegisterTest<test::TestBatchRender>("Batch Render - Objects");
        testMenu->RegisterTest<test::TestBatchRenderColors>("Batch Render - Colors");
        testMenu->RegisterTest<test::TestBatchRenderTextures>("Batch Render - Textures");
        testMenu->RegisterTest<test::TestBatchRenderDynamic>("Batch Render - Dynamic");

    	
        //Clear
        //va.Unbind();
        //shader.Unbind();
        //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


        /* Loop until the user closes the window */
        while(!glfwWindowShouldClose(window)) {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
            renderer.Clear();

            //ImGui New Frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

        	if(currentTest) {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();

                ImGui::Begin("Test");
        		if(currentTest != testMenu && ImGui::Button("<-")) {
                    delete currentTest;
                    currentTest = testMenu;
        		}
                currentTest->OnImGuiRender();
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

        delete currentTest;

        if(currentTest != testMenu)
            delete testMenu;
    }

    //Shutdown
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
