
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
    window = glfwCreateWindow(960, 540, "Hello OpenGL", NULL, NULL);
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
            // 顶点位置      // 纹理坐标    
			0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
			520.0f, 0.0f,  0.0f,  1.0f,  0.0f,
			520.0f,  235.0f,  0.0f,  1.0f,  1.0f,
			0.0f,  235.0f,  0.0f,  0.0f,  1.0f,
        };

		// 索引数据
        unsigned int indexes[] = {
		    0, 1, 2,
            2, 3, 0
        };
        /*
		VertexArray是顶点数组对象（VAO），用于存储顶点缓冲区对象（VBO）和索引缓冲区对象（IBO）的状态。
		VertexBuffer是顶点缓冲区对象（VBO），用于存储顶点数据并发送至VRAM， 但是不解释顶点数据的布局和含义。
		IndexBuffer是索引缓冲区对象（IBO），用于存储索引数据，允许我们重用顶点数据来绘制图形，减少VRAM的使用。
		VertexBufferLayout是一个顶点缓冲区布局，定义了顶点数据的格式和布局，向OpenGL解释如何处理顶点数据。
        
		一个（VAO）可以绑定多个（VBO）和（IBO），每个（VBO）可以有自己的布局（VertexBufferLayout）。
        */
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

        Shader GLshader("res/shaders/Basic.shader");
        GLshader.bind();

		glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		// 计算模型(model)视图(view)投影(projection)矩阵，也就是MVP矩阵，在OpenGL中实际计算顺序是P * V * M
        //glm::mat4 mvp = projection * view * model;
        
		GLCall(glEnable(GL_BLEND)); // 开启混合
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // 设置混合函数

		Texture texture("res/textures/OpenGL.png");
		texture.bind(0);
		GLshader.setUniform1i("u_Texture", 0); // 设置纹理单元
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

        glm::vec3 translation(0, 0, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* 在这里渲染 */
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
			// ImGui 渲染
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