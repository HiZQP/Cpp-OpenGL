#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); // 翻转纹理坐标
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // 4表示加载RGBA格式的纹理

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer); // 释放加载的纹理数据
		m_LocalBuffer = nullptr;
	}
	else
	{
		LOG(LogLevel::LOG_LEVEL_ERROR, "纹理加载失败: " + path);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // 激活纹理单元
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID)); // 绑定纹理
	LOG(LogLevel::LOG_LEVEL_INFO, "纹理绑定完成: " + m_FilePath);
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // 解绑纹理
	LOG(LogLevel::LOG_LEVEL_INFO, "纹理解绑完成: " + m_FilePath);
}
