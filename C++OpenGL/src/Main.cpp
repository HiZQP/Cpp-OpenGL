
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
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Input.h"
#include "Camera.h"
#include "Models/Models.h"

int main(void)
{
    LogSystem::getInstance().init("logs/", "OpenGL", LOG_DISABLE);

    GLFWwindow* window;

    // ��ʼ��GLFW��
    if (!glfwInit())
    {
        LOG(LogLevel::LOG_LEVEL_FATAL, "GLFW ��ʼ��ʧ��");
        return -1;
    }
    else {
        LOG(LogLevel::LOG_LEVEL_INFO, "GLFW ��ʼ�����");
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

    LOG(LogLevel::LOG_LEVEL_INFO, "OpenGL �汾: " + std::string((const char*)glGetString(GL_VERSION)));

    // ��ʼ��GLEW
    if (glewInit() != GLEW_OK)
        LOG(LogLevel::LOG_LEVEL_FATAL, "GLEW ��ʼ��ʧ��");
    else
        LOG(LogLevel::LOG_LEVEL_INFO, "GLEW ��ʼ�����");

    {///////////////////////////////////////////////////////////
        /*
        // ��������
        float vertexs[] = {
            // ����λ��      // ��������    
			  0.0f,    0.0f,  0.0f,  0.0f,  0.0f,
			520.0f,    0.0f,  0.0f,  1.0f,  0.0f,
			520.0f,  235.0f,  0.0f,  1.0f,  1.0f,
			  0.0f,  235.0f,  0.0f,  0.0f,  1.0f,
        };

		// ��������
        unsigned int indexes[] = {
		    0, 1, 2,
            2, 3, 0
        };
        
		VertexArray�Ƕ����������VAO�������ڴ洢���㻺��������VBO������������������IBO����״̬��
		VertexBuffer�Ƕ��㻺��������VBO�������ڴ洢�������ݲ�������VRAM�� ���ǲ����Ͷ������ݵĲ��ֺͺ��塣
		IndexBuffer����������������IBO�������ڴ洢�������ݣ������������ö�������������ͼ�Σ�����VRAM��ʹ�á�
		VertexBufferLayout��һ�����㻺�������֣������˶������ݵĸ�ʽ�Ͳ��֣���OpenGL������δ��������ݡ�
        
		һ����VAO�����԰󶨶����VBO���ͣ�IBO����ÿ����VBO���������Լ��Ĳ��֣�VertexBufferLayout����
        
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
        */

        Model building("res/Meshes/building.obj");

        Shader GLshader("res/shaders/Basic.shader");
        GLshader.bind();

		GLCall(glEnable(GL_BLEND)); // �������
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // ���û�Ϻ���

		//Texture texture("res/textures/OpenGL.png");
		//texture.bind(0);
		//GLshader.setUniform1i("u_Texture", 0); // ��������Ԫ
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

		Renderer renderer;
        Input input(window);
		Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), 960.0f / 540.0f, 60.0f, 0.1f, 1000.0f);
        float scale = 1.0f;
        glm::vec3 translate = glm::vec3(0.0f);

        glEnable(GL_DEPTH_TEST); // ������Ȳ���
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ����ʱͬʱ�����Ȼ�����

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* ��������Ⱦ */
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

			renderer.clear();

			// ��������
            if(input.isKeyPressed(GLFW_KEY_ESCAPE)) {
                glfwSetWindowShouldClose(window, true);
			}
            if(input.isKeyPressed(GLFW_KEY_W)) {
                camera.move(glm::vec3(0.0f, 0.0f, -1.0f));
			}
            if(input.isKeyPressed(GLFW_KEY_S)) {
				camera.move(glm::vec3(0.0f, 0.0f, 1.0f));
			}
			if (input.isKeyPressed(GLFW_KEY_A)) {
				camera.move(glm::vec3(-1.0f, 0.0f, 0.0f));
			}
			if (input.isKeyPressed(GLFW_KEY_D)) {
				camera.move(glm::vec3(1.0f, 0.0f, 0.0f));
			}
			if (input.isKeyPressed(GLFW_KEY_Q)) {
				camera.move(glm::vec3(0.0f, -1.0f, 0.0f));
			}
			if (input.isKeyPressed(GLFW_KEY_E)) {
				camera.move(glm::vec3(0.0f, 1.0f, 0.0f));
			}
            if (input.isKeyPressed(GLFW_KEY_J)) {
                camera.rotate(glm::vec3(0.0f, 1.0f, 0.0f));
            }
			// �����ٶȿ��������ת
                glm::vec3 mouseAccel = input.getMouseAcceleration();
            if (input.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
                camera.rotate(glm::vec3(mouseAccel.y, mouseAccel.x, 0.0f)); // �����ٶȿ��������ת
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, translate);
            model = glm::scale(model, glm::vec3(scale));

            glm::mat4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix() * model;

            GLshader.setUniformMat4f("u_MVP", mvp);

            building.draw(GLshader);

            //glClear(GL_COLOR_BUFFER_BIT);
            
			// ImGui ��Ⱦ
            ///////////////////////////////////////////////////////
            {
                ImGui::Begin("OpenGL_Test");
                //ImGui::SliderFloat3("Translation", &translate.x, -500.0f, 500.0f);
				ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);
				// ��ʾ���λ��
				ImGui::Text("MouseX: %f MouseY: %f", input.getMousePosition().x, input.getMousePosition().y);
                // ��ʾ�����ٶ�
				ImGui::Text("Mouse Acceleration: X: %.3f Y: %.3f Z: %.3f", mouseAccel.x, mouseAccel.y, mouseAccel.z);

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