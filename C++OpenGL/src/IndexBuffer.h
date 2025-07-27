#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const void* data, unsigned int size);

	void deleteIt() const;

	void bind() const;

	void unbind() const;

	inline unsigned int getCount() const { return m_Count; }
	inline unsigned int getRendererID() const { return m_RendererID; }
};

