
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

    // ��ʼ��GLFW��
    if (!glfwInit())
    {
        LOG(LogLevel::LOG_LEVEL_FATAL, "Failed to initialize GLFW");
        return -1;
    }
    else {
        LOG(LogLevel::LOG_LEVEL_INFO, "GLFW initialized successfully");
    }

    // ʹ��OpenGL 3.3 ���������ļ�
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

    // ��ʼ��GLEW
    if (glewInit() != GLEW_OK)
        LOG(LogLevel::LOG_LEVEL_FATAL, "Failed to initialize GLEW");
    else
        LOG(LogLevel::LOG_LEVEL_INFO, "GLEW initialized successfully");

    {///////////////////////////////////////////////////////////
        // ��������
        float vertexs[] = {
            // ����λ��          // ��ɫ
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        };

		// ��������
        unsigned int indexes[] = {
            0, 1, 2,
        };
        /*
		VertexArray��һ�������������VAO���������ڴ洢���㻺��������VBO������������������IBO����״̬��
		VertexBuffer��һ�����㻺��������VBO���������ڴ洢�������ݣ� ���ǲ����Ͷ������ݵĲ��ֺͺ��塣
		IndexBuffer��һ����������������IBO���������ڴ洢�������ݣ������������ö�������������ͼ�Ρ�
		VertexBufferLayout��һ�����㻺�������֣��������˶������ݵĸ�ʽ�Ͳ��֣���OpenGL������δ��������ݡ�
        
		һ����VAO�����԰󶨶����VBO���ͣ�IBO����ÿ����VBO���������Լ��Ĳ��֣�VertexBufferLayout����
        */
        // �������㻺�������� (VBO) ���������������� (IBO)
        VertexBuffer vb(vertexs, sizeof(vertexs));

        // ��������������
        IndexBuffer ib(indexes, sizeof(indexes) / sizeof(unsigned int));

        // �������㻺��������
		VertexBufferLayout layout;
		layout.push<float>(3); // ����λ��
		layout.push<float>(3); // ��ɫ

		// ��������������� (VAO)
		VertexArray va;
		va.addBuffer(vb, layout);

        GLCall(glBindVertexArray(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        Shader GLshader("res/shader/Basic.shader");
        GLshader.bind();

        glfwSwapInterval(1); // ���ô�ֱͬ����1��ʾ������ֱͬ��
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // ����������ɫ

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* ��������Ⱦ */
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