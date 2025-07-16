
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "LogSystem.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"

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
    window = glfwCreateWindow(960, 540, "Hello OpenGL", NULL, NULL);
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
            // ����λ��      // ��������    
			0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
			520.0f, 0.0f,  0.0f,  1.0f,  0.0f,
			520.0f,  235.0f,  0.0f,  1.0f,  1.0f,
			0.0f,  235.0f,  0.0f,  0.0f,  1.0f,
        };

		// ��������
        unsigned int indexes[] = {
		    0, 1, 2,
            2, 3, 0
        };
        /*
		VertexArray�Ƕ����������VAO�������ڴ洢���㻺��������VBO������������������IBO����״̬��
		VertexBuffer�Ƕ��㻺��������VBO�������ڴ洢�������ݲ�������VRAM�� ���ǲ����Ͷ������ݵĲ��ֺͺ��塣
		IndexBuffer����������������IBO�������ڴ洢�������ݣ������������ö�������������ͼ�Σ�����VRAM��ʹ�á�
		VertexBufferLayout��һ�����㻺�������֣������˶������ݵĸ�ʽ�Ͳ��֣���OpenGL������δ��������ݡ�
        
		һ����VAO�����԰󶨶����VBO���ͣ�IBO����ÿ����VBO���������Լ��Ĳ��֣�VertexBufferLayout����
        */
        // �������㻺�������� (VBO) ���������������� (IBO)
        VertexBuffer vb(vertexs, sizeof(vertexs));

        // ��������������
        IndexBuffer ib(indexes, sizeof(indexes) / sizeof(unsigned int));

        // �������㻺��������
		VertexBufferLayout layout;
		layout.push<float>(3); // ����λ��
		layout.push<float>(2); // ��������

		// ��������������� (VAO)
		VertexArray va;
		va.addBuffer(vb, layout);

        Shader GLshader("res/shaders/Basic.shader");
        GLshader.bind();

		glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		// ����ģ��(model)��ͼ(view)ͶӰ(projection)����Ҳ����MVP������OpenGL��ʵ�ʼ���˳����P * V * M
        //glm::mat4 mvp = projection * view * model;
        
		GLCall(glEnable(GL_BLEND)); // �������
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // ���û�Ϻ���

		Texture texture("res/textures/OpenGL.png");
		texture.bind(0);
		GLshader.setUniform1i("u_Texture", 0); // ��������Ԫ
		// ImGui ��ʼ��
        ///////////////////////////////////////////////////////
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        bool show_demo_window = true;
        bool show_another_window = false;
        ///////////////////////////////////////////////////////

        glfwSwapInterval(1); // ���ô�ֱͬ����1��ʾ������ֱͬ��
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // ����������ɫ

        glm::vec3 translation(0, 0, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* ��������Ⱦ */
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation );
            glm::mat4 mvp = projection * view * model;

			va.bind();
			ib.bind();

            GLshader.setUniformMat4f("u_MVP", mvp);

            glClear(GL_COLOR_BUFFER_BIT);

            GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
			// ImGui ��Ⱦ
            ///////////////////////////////////////////////////////
            {
                ImGui::Begin("Hello, world!");
				ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            ///////////////////////////////////////////////////////
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        GLshader.unbind();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}