#pragma once

#include "Renderer.h"

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);

	~VertexBuffer();

	void bind() const;

	void unbind() const;

	inline unsigned int getRendererID() const { return m_RendererID; }
};

