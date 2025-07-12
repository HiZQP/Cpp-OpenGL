
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "LogSystem.h"
#include "Renderer.h"
#include "Shader.h"

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

    ///////////////////////////////////////////////////////////
    

    //三角形
    float positions[] = {
		// 顶点位置          // 颜色
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned int indexes[] = {
		0, 1, 2,
	};
    //创建顶点数组
	unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

	// 创建顶点缓冲区对象 (VBO) 和索引缓冲区对象 (IBO)
    unsigned int vbo;
    GLCall(glGenBuffers(1, &vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
	
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));
    
    // 创建索引缓冲区
    unsigned int ebo;
    GLCall(glGenBuffers(1, &ebo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW));

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
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glBindVertexArray(vao));

        GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	GLshader.unbind();

    glfwTerminate();
    return 0;
}