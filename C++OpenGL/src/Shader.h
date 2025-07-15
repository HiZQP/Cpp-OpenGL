#pragma once

#include <unordered_map>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "LogSystem.h"

class Shader
{
private:
    struct ShaderProgramSource {
        std::string vertexSource;
        std::string fragmentSource;
    };
	unsigned int m_programeID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:

	Shader(const std::string& filePath);


	~Shader();
	
	Shader(const Shader&) = delete; // ��ֹ�������캯��
	
	Shader& operator=(const Shader&) = delete; // ��ֹ��ֵ������

	void bind() const;

	void unbind() const;

    ShaderProgramSource parseShader(const std::string& filePath);

	void setUniform1i(const std::string& name, int value);

	void setUniform1f(const std::string& name, float value);

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
    unsigned int compileShader(unsigned int type, const std::string& source);

    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int getUniformLocation(const std::string& name);
};

