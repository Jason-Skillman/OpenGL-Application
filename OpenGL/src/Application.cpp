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

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
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
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f
        };
        unsigned int indices[]{
            0, 1, 2,
            2, 3, 0
        };



        //----- VertexArray/Buffer -----
        VertexArray va;
    	
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    	
        VertexBufferLayout layout;
        layout.Push<float>(2);

    	//Add the buffer and layout to the vertex array
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);



        //----- Shader -----
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);


        //Clear
        va.Unbind();
        shader.Unbind();
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));



        //----- Renderer -----
        Renderer renderer;



        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while(!glfwWindowShouldClose(window)) {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            //shader.Bind();
            //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
            
            //va.Bind();
            //vb.Bind();
            //ib.Bind();

            
            renderer.Draw(va, ib, shader);


        	//Animation
            /*if(r > 1.0f) increment = -0.5f;
            if(r < 1.0f) increment = 0.5f;
            r += increment;*/


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
