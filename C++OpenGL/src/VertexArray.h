#pragma once

#include"VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID; // OpenGL渲染器ID
	// 禁止拷贝构造和赋值
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	// 禁止移动构造和赋值
	VertexArray(VertexArray&&) = delete;
	VertexArray& operator=(VertexArray&&) = delete;
public:

	VertexArray();

	void deleteIt() const;

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;

	void unbind() const;

	inline unsigned int getRendererID() const { return m_RendererID;  }
};

