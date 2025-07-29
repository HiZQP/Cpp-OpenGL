#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); // 翻转纹理坐标
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0); // 0表示自动选择通道数
	
	if(!m_LocalBuffer) {
		LOG(LogLevel::LOG_LEVEL_ERROR, "纹理加载失败: " + path);
		return;
	}

	GLenum format = 0;
	switch (m_BPP) {
	case 1: format = GL_RED; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default:
		stbi_image_free(m_LocalBuffer);
		LOG(LogLevel::LOG_LEVEL_ERROR, "不支持的纹理格式: " + path);
		return;
	}

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//GLCall(glGenerateMipmap(GL_TEXTURE_2D)); // 生成多级渐远纹理

	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // 解绑纹理
	stbi_image_free(m_LocalBuffer); // 释放加载的纹理数据		
}

Texture::~Texture()
{
}

void Texture::bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // 激活纹理单元
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID)); // 绑定纹理
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // 解绑纹理
}
