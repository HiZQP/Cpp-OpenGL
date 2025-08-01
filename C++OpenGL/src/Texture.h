#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path);
	
	~Texture();

	void bind(unsigned int slot) const;
	
	void unbind() const;

	inline int getWidth() const { return m_Width; }

	inline int getHeight() const { return m_Height; }

	inline unsigned int getRendererID() const { return m_RendererID; }
};