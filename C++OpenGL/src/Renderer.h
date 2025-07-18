#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "LogSystem.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// 定义断言宏和OpenGL调用宏
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
private:

public:

    void clear() const;

	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};