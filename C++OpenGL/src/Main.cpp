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
#include "SceneLight.h"
#include "Events/Event.h"
#include "Events/InputEvent.h"




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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

	// ��ʼ���¼�������������
	auto eventPublisher = std::make_shared<EventPublisher>();
    Input::getInstance().init(window, eventPublisher);

    LOG(LogLevel::LOG_LEVEL_INFO, "OpenGL �汾: " + std::string((const char*)glGetString(GL_VERSION)));

    // ��ʼ��GLEW
    if (glewInit() != GLEW_OK)
        LOG(LogLevel::LOG_LEVEL_FATAL, "GLEW ��ʼ��ʧ��");
    else
        LOG(LogLevel::LOG_LEVEL_INFO, "GLEW ��ʼ�����");

    {///////////////////////////////////////////////////////////
        /*
	    VertexArray�Ƕ����������VAO�������ڴ洢���㻺��������VBO������������������IBO����״̬��
	    VertexBuffer�Ƕ��㻺��������VBO�������ڴ洢�������ݲ�������VRAM�� ���ǲ����Ͷ������ݵĲ��ֺͺ��塣
		IndexBuffer����������������IBO�������ڴ洢�������ݣ������������ö�������������ͼ�Σ�����VRAM��ʹ�á�
		VertexBufferLayout��һ�����㻺�������֣������˶������ݵĸ�ʽ�Ͳ��֣���OpenGL������δ��������ݡ�
        */

        Shader GLshader("res/shaders/Basic.shader");
        GLshader.bind();

		GLCall(glEnable(GL_BLEND)); // �������
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // ���û�Ϻ���
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

		GLCall(glEnable(GL_CULL_FACE)); // ���������޳�

        glfwSwapInterval(1); // ���ô�ֱͬ����1��ʾ������ֱͬ��
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // ����������ɫ

		Renderer renderer;
		Camera camera(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 960.0f / 540.0f, 60.0f, 0.1f, 1000.0f);
        float scale = 1.0f;
        glm::vec3 translate = glm::vec3(0.0f);
		glm::vec3 rotate = glm::vec3(0.0f);
        float cameraSpeed = 0.1f;
		float FOV = 60.0f; // �����Ұ��Χ
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec4 ambientColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
        Model ball("res/Meshes/RustyMetalBall/ball.fbx");

		// ���ù�Դ
		DirectionalLight directionalLight;
		directionalLight.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // ��ɫ��
		directionalLight.direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f); // ���Ϸ�����
		SceneLightManager sceneLightManager;
		sceneLightManager.addLight(directionalLight);
		// ���¹�Դ����
		sceneLightManager.updateDirectionalLights();


		// ��Ⱦѭ��
        while (!glfwWindowShouldClose(window))
        {
			Input::getInstance().updateRawInputInLoop();

            /* ��������Ⱦ */
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

			renderer.clear();

            eventPublisher.subscribe<KeyEvent>([](const KeyEvent& event) {
                if (event.getKeyAction() == KeyEvent::KeyAction::Press) {
                    LOG(LogLevel::LOG_LEVEL_INFO, "Key Pressed: " + std::to_string(event.getKey()));
                }
				});

            camera.move(direction);
            camera.setSpeed(cameraSpeed);
			camera.setFov(FOV);
			// �����ٶȿ��������ת
            
            glm::vec3 mouseAccel = glm::vec3(0.0f, 0.0f, 0.0f);

            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 projection = camera.getProjectionMatrix();
			// ����ƽ��
            model = glm::translate(model, translate);
            // ������ת
			model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
			// ��������
            model = glm::scale(model, glm::vec3(scale));
            
            GLshader.setUniformMat4f("u_Model", model);
            GLshader.setUniformMat4f("u_View", view);
            GLshader.setUniformMat4f("u_Projection", projection);

            GLshader.setUniform4f("u_Ambient", ambientColor.x, ambientColor.y, ambientColor.z, ambientColor.w);

			GLshader.setUniform3f("u_CameraPos", camera.getPos().x, camera.getPos().y, camera.getPos().z);

            ball.draw(GLshader);
            
			// ImGui ��Ⱦ
            ///////////////////////////////////////////////////////
            {
                ImGui::Begin("OpenGL_Test");
                ImGui::SliderFloat3("Translation", &translate.x, -50.0f, 50.0f);
				ImGui::SliderFloat3("Rotation", &rotate.x, -180.0f, 180.0f);
				ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);

				ImGui::ColorPicker4("Ambient Color", &ambientColor.x);
				// ��ʾ���λ��
				//ImGui::Text("MouseX: %f MouseY: %f", input.getMousePosition().x, input.getMousePosition().y);
                // ��ʾ�����ٶ�
				ImGui::Text("Mouse Acceleration: X: %.3f Y: %.3f Z: %.3f", mouseAccel.x, mouseAccel.y, mouseAccel.z);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();

				ImGui::Begin("Camera");
				ImGui::SliderFloat("FOV", &FOV, 5.0f, 120.0f);
                ImGui::SliderFloat("Camera Speed", &cameraSpeed, 0.01f, 0.2f);
				ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", camera.getPos().x, camera.getPos().y, camera.getPos().z);
				ImGui::Text("Camera Rotation: (%.2f, %.2f, %.2f)", camera.getRot().x, camera.getRot().y, camera.getRot().z);
				ImGui::Text("Camera Front: (%.2f, %.2f, %.2f)", camera.getFront().x, camera.getFront().y, camera.getFront().z);
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