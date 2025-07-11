#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "LogSystem.h"

// ������Ժ��OpenGL���ú�
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private:
public:
	Renderer();

	~Renderer();
};
