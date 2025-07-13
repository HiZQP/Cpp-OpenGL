#pragma once

#include <vector>

#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;// 数据类型
	unsigned int count;// 数据分量数
	unsigned char normalized;// 是否归一化
	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{   
			case GL_FLOAT:          return sizeof(float);
			case GL_UNSIGNED_INT:   return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE:  return sizeof(unsigned char);
		}
		ASSERT(false, "Unknown type!");
		return 0;
	}
};	

class VertexBufferLayout
{
private:
	unsigned int m_Stride;
	std::vector<VertexBufferElement> m_Elements;

public:
	VertexBufferLayout()
		: m_Stride(0) { }

	template<typename T>
	void push(unsigned int couunt) {
		static_assert(false, "Type not supported!");
	}
	
	template<>
	void push<float>(unsigned int count) {
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}
	
	template<>
	void push<unsigned int>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}
	
	template<>
	void push<unsigned char>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}


	inline const std::vector<VertexBufferElement>& getElements() const { return m_Elements; }

	inline unsigned int getStride() const { return m_Stride; }

};

