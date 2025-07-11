
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "LogSystem.h"
#include "Renderer.h"
#include "Shader.h"

// ��־ϵͳʵ��

Renderer OpenGLRenderer;


int main(void)
{
    LogSystem::getInstance().init("logs/", "OpenGL", true);

    GLFWwindow* window;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    ///////////////////////////////////////////////////////////
    
    //������
    float positions[] = {
		// ����λ��          // ��ɫ
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned int indexes[] = {
		0, 1, 2,
	};
    //������������
	unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

	// �������㻺�������� (VBO) ���������������� (IBO)
    unsigned int vbo;
    GLCall(glGenBuffers(1, &vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
	
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));
    
    // ��������������
    unsigned int ebo;
    GLCall(glGenBuffers(1, &ebo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW));

    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    ShaderProgramSource source = parseShader("res/shader/Basic.shader");
	logSystem.log(LogSystem::LogLevel::LOG_LEVEL_INFO, "Vertex Shader Source:\n" + source.vertexSource);
	logSystem.log(LogSystem::LogLevel::LOG_LEVEL_INFO, "Fragment Shader Source:\n" + source.fragmentSource);

    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);

	glfwSwapInterval(1); // ���ô�ֱͬ����1��ʾ������ֱͬ��
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // ����������ɫ


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glUseProgram(shader));
        GLCall(glBindVertexArray(vao));

        GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteShader(shader);

    glfwTerminate();
    return 0;
}