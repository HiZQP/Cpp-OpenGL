#pragma once

#include"VertexBuffer.h"

class VertexBufferLayout;

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

	void deleteIt() const;

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;

	void unbind() const;

	inline unsigned int getRendererID() const { return m_RendererID;  }
};

