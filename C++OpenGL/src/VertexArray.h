#pragma once

#include "VertexBufferLayout.h" 
#include"VertexBuffer.h"

class VertexArray
{
private:
	unsigned int m_RendererID; // OpenGL��Ⱦ��ID
	// ��ֹ��������͸�ֵ
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	// ��ֹ�ƶ�����͸�ֵ
	VertexArray(VertexArray&&) = delete;
	VertexArray& operator=(VertexArray&&) = delete;
public:

	VertexArray();

	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;

	void unbind() const;
};

