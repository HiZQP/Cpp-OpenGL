
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "LogSystem.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

int main(void)
{
    LogSystem::getInstance().init("logs/", "OpenGL", LOG_DISABLE);

    GLFWwindow* window;

    // 初始化GLFW库
    if (!glfwInit())
    {
        LOG(LogLevel::LOG_LEVEL_FATAL, "Failed to initialize GLFW");
        return -1;
    }
    else {
        LOG(LogLevel::LOG_LEVEL_INFO, "GLFW initialized successfully");
    }

    // 使用OpenGL 3.3 核心配置文件
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    LOG(LogLevel::LOG_LEVEL_INFO, "OpenGL Version: " + std::string((const char*)glGetString(GL_VERSION)));

    // 初始化GLEW
    if (glewInit() != GLEW_OK)
        LOG(LogLevel::LOG_LEVEL_FATAL, "Failed to initialize GLEW");
    else
        LOG(LogLevel::LOG_LEVEL_INFO, "GLEW initialized successfully");

    {///////////////////////////////////////////////////////////
        // 顶点数据
        float vertexs[] = {
            // 顶点位置          // 颜色
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        };

		// 索引数据
        unsigned int indexes[] = {
            0, 1, 2,
        };
        /*
		VertexArray是一个顶点数组对象（VAO），它用于存储顶点缓冲区对象（VBO）和索引缓冲区对象（IBO）的状态。
		VertexBuffer是一个顶点缓冲区对象（VBO），它用于存储顶点数据， 但是不解释顶点数据的布局和含义。
		IndexBuffer是一个索引缓冲区对象（IBO），它用于存储索引数据，允许我们重用顶点数据来绘制图形。
		VertexBufferLayout是一个顶点缓冲区布局，它定义了顶点数据的格式和布局，向OpenGL解释如何处理顶点数据。
        
		一个（VAO）可以绑定多个（VBO）和（IBO），每个（VBO）可以有自己的布局（VertexBufferLayout）。
        */
        // 创建顶点缓冲区对象 (VBO) 和索引缓冲区对象 (IBO)
        VertexBuffer vb(vertexs, sizeof(vertexs));

        // 创建索引缓冲区
        IndexBuffer ib(indexes, sizeof(indexes) / sizeof(unsigned int));

        // 创建顶点缓冲区布局
		VertexBufferLayout layout;
		layout.push<float>(3); // 顶点位置
		layout.push<float>(3); // 颜色

		// 创建顶点数组对象 (VAO)
		VertexArray va;
		va.addBuffer(vb, layout);

        GLCall(glBindVertexArray(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        Shader GLshader("res/shader/Basic.shader");
        GLshader.bind();

        glfwSwapInterval(1); // 设置垂直同步，1表示开启垂直同步
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // 设置清屏颜色

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* 在这里渲染 */
			va.bind();
			ib.bind();

            glClear(GL_COLOR_BUFFER_BIT);

            GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr));

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        GLshader.unbind();
    }
    glfwTerminate();
    return 0;
}