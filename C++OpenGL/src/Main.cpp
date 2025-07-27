
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

    LOG(LogLevel::LOG_LEVEL_INFO, "OpenGL 版本: " + std::string((const char*)glGetString(GL_VERSION)));

    // 初始化GLEW
    if (glewInit() != GLEW_OK)
        LOG(LogLevel::LOG_LEVEL_FATAL, "GLEW 初始化失败");
    else
        LOG(LogLevel::LOG_LEVEL_INFO, "GLEW 初始化完成");

    {///////////////////////////////////////////////////////////
        /*
        // 顶点数据
        float vertexs[] = {
            // 顶点位置      // 纹理坐标    
			  0.0f,    0.0f,  0.0f,  0.0f,  0.0f,
			520.0f,    0.0f,  0.0f,  1.0f,  0.0f,
			520.0f,  235.0f,  0.0f,  1.0f,  1.0f,
			  0.0f,  235.0f,  0.0f,  0.0f,  1.0f,
        };

		// 索引数据
        unsigned int indexes[] = {
		    0, 1, 2,
            2, 3, 0
        };
        
		VertexArray是顶点数组对象（VAO），用于存储顶点缓冲区对象（VBO）和索引缓冲区对象（IBO）的状态。
		VertexBuffer是顶点缓冲区对象（VBO），用于存储顶点数据并发送至VRAM， 但是不解释顶点数据的布局和含义。
		IndexBuffer是索引缓冲区对象（IBO），用于存储索引数据，允许我们重用顶点数据来绘制图形，减少VRAM的使用。
		VertexBufferLayout是一个顶点缓冲区布局，定义了顶点数据的格式和布局，向OpenGL解释如何处理顶点数据。
        
		一个（VAO）可以绑定多个（VBO）和（IBO），每个（VBO）可以有自己的布局（VertexBufferLayout）。
        
        // 创建顶点缓冲区对象 (VBO) 和索引缓冲区对象 (IBO)
        VertexBuffer vb(vertexs, sizeof(vertexs));

        // 创建索引缓冲区
        IndexBuffer ib(indexes, sizeof(indexes) / sizeof(unsigned int));

        // 创建顶点缓冲区布局
		VertexBufferLayout layout;
		layout.push<float>(3); // 顶点位置
		layout.push<float>(2); // 纹理坐标

		// 创建顶点数组对象 (VAO)
		VertexArray va;                                      
		va.addBuffer(vb, layout);
        */

        Model building("res/Meshes/building.obj");

        Shader GLshader("res/shaders/Basic.shader");
        GLshader.bind();

		GLCall(glEnable(GL_BLEND)); // 开启混合
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // 设置混合函数

		//Texture texture("res/textures/OpenGL.png");
		//texture.bind(0);
		//GLshader.setUniform1i("u_Texture", 0); // 设置纹理单元
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

        glfwSwapInterval(1); // 设置垂直同步，1表示开启垂直同步
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // 设置清屏颜色

		Renderer renderer;
        Input input(window);
		Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), 960.0f / 540.0f, 60.0f, 0.1f, 1000.0f);
        float scale = 1.0f;
        glm::vec3 translate = glm::vec3(0.0f);

        glEnable(GL_DEPTH_TEST); // 启用深度测试
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清屏时同时清除深度缓冲区

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* 在这里渲染 */
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

			renderer.clear();

			// 处理输入
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
			// 鼠标加速度控制相机旋转
                glm::vec3 mouseAccel = input.getMouseAcceleration();
            if (input.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
                camera.rotate(glm::vec3(mouseAccel.y, mouseAccel.x, 0.0f)); // 鼠标加速度控制相机旋转
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, translate);
            model = glm::scale(model, glm::vec3(scale));

            glm::mat4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix() * model;

            GLshader.setUniformMat4f("u_MVP", mvp);

            building.draw(GLshader);

            //glClear(GL_COLOR_BUFFER_BIT);
            
			// ImGui 渲染
            ///////////////////////////////////////////////////////
            {
                ImGui::Begin("OpenGL_Test");
                //ImGui::SliderFloat3("Translation", &translate.x, -500.0f, 500.0f);
				ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);
				// 显示鼠标位置
				ImGui::Text("MouseX: %f MouseY: %f", input.getMousePosition().x, input.getMousePosition().y);
                // 显示鼠标加速度
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