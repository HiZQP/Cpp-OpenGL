#include "Renderer.h"

Renderer::Renderer()
{
// 初始化GLFW库
if (!glfwInit())
{
	LOG(LogLevel::LOG_LEVEL_FATAL, "Failed to initialize GLFW");
	return;
}
if (glewInit() != GLEW_OK)
	LOG(LogLevel::LOG_LEVEL_FATAL, "Failed to initialize GLEW");
else
	LOG(LogLevel::LOG_LEVEL_INFO, "GLEW initialized successfully");

// 使用OpenGL 3.3 核心配置文件
GLCall(glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3));
GLCall(glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3));
GLCall(glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE));
}

Renderer::~Renderer()
{
// 终止GLFW
glfwTerminate();
	LOG(LogLevel::LOG_LEVEL_INFO, "GLFW terminated successfully");
}

void GLClearError() {
while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
while (GLenum error = glGetError()) {
	LOG(LogLevel::LOG_LEVEL_FATAL,
		" (" + std::to_string(error) + "): " + function +
		" " + file + ":" + std::to_string(line));
	return false;
}
return true;
}