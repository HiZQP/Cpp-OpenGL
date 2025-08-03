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

    // 初始化GLFW库
    if (!glfwInit())
    {
        LOG(LogLevel::LOG_LEVEL_FATAL, "GLFW 初始化失败");
        return -1;
    }
    else {
        LOG(LogLevel::LOG_LEVEL_INFO, "GLFW 初始化完成");
    }

    // 使用OpenGL 3.3 核心配置文件
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

	// 初始化事件管理和输入管理
	auto eventPublisher = std::make_shared<EventPublisher>();
    Input::getInstance().init(window, eventPublisher);

    LOG(LogLevel::LOG_LEVEL_INFO, "OpenGL 版本: " + std::string((const char*)glGetString(GL_VERSION)));

    // 初始化GLEW
    if (glewInit() != GLEW_OK)
        LOG(LogLevel::LOG_LEVEL_FATAL, "GLEW 初始化失败");
    else
        LOG(LogLevel::LOG_LEVEL_INFO, "GLEW 初始化完成");

    {///////////////////////////////////////////////////////////
        /*
	    VertexArray是顶点数组对象（VAO），用于存储顶点缓冲区对象（VBO）和索引缓冲区对象（IBO）的状态。
	    VertexBuffer是顶点缓冲区对象（VBO），用于存储顶点数据并发送至VRAM， 但是不解释顶点数据的布局和含义。
		IndexBuffer是索引缓冲区对象（IBO），用于存储索引数据，允许我们重用顶点数据来绘制图形，减少VRAM的使用。
		VertexBufferLayout是一个顶点缓冲区布局，定义了顶点数据的格式和布局，向OpenGL解释如何处理顶点数据。
        */

        Shader GLshader("res/shaders/Basic.shader");
        GLshader.bind();

		GLCall(glEnable(GL_BLEND)); // 开启混合
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // 设置混合函数
		// ImGui 初始化
        ///////////////////////////////////////////////////////
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        bool show_demo_window = true;
        bool show_another_window = false;
        ///////////////////////////////////////////////////////

		GLCall(glEnable(GL_CULL_FACE)); // 开启背面剔除

        glfwSwapInterval(1); // 设置垂直同步，1表示开启垂直同步
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // 设置清屏颜色

		Renderer renderer;
		Camera camera(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 960.0f / 540.0f, 60.0f, 0.1f, 1000.0f);
        float scale = 1.0f;
        glm::vec3 translate = glm::vec3(0.0f);
		glm::vec3 rotate = glm::vec3(0.0f);
        float cameraSpeed = 0.1f;
		float FOV = 60.0f; // 相机视野范围
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec4 ambientColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
        Model ball("res/Meshes/RustyMetalBall/ball.fbx");

		// 设置光源
		DirectionalLight directionalLight;
		directionalLight.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // 白色光
		directionalLight.direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f); // 从上方照射
		SceneLightManager sceneLightManager;
		sceneLightManager.addLight(directionalLight);
		// 更新光源数据
		sceneLightManager.updateDirectionalLights();


		// 渲染循环
        while (!glfwWindowShouldClose(window))
        {
			Input::getInstance().updateRawInputInLoop();

            /* 在这里渲染 */
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
			// 鼠标加速度控制相机旋转
            
            glm::vec3 mouseAccel = glm::vec3(0.0f, 0.0f, 0.0f);

            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 projection = camera.getProjectionMatrix();
			// 设置平移
            model = glm::translate(model, translate);
            // 设置旋转
			model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
			// 设置缩放
            model = glm::scale(model, glm::vec3(scale));
            
            GLshader.setUniformMat4f("u_Model", model);
            GLshader.setUniformMat4f("u_View", view);
            GLshader.setUniformMat4f("u_Projection", projection);

            GLshader.setUniform4f("u_Ambient", ambientColor.x, ambientColor.y, ambientColor.z, ambientColor.w);

			GLshader.setUniform3f("u_CameraPos", camera.getPos().x, camera.getPos().y, camera.getPos().z);

            ball.draw(GLshader);
            
			// ImGui 渲染
            ///////////////////////////////////////////////////////
            {
                ImGui::Begin("OpenGL_Test");
                ImGui::SliderFloat3("Translation", &translate.x, -50.0f, 50.0f);
				ImGui::SliderFloat3("Rotation", &rotate.x, -180.0f, 180.0f);
				ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);

				ImGui::ColorPicker4("Ambient Color", &ambientColor.x);
				// 显示鼠标位置
				//ImGui::Text("MouseX: %f MouseY: %f", input.getMousePosition().x, input.getMousePosition().y);
                // 显示鼠标加速度
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