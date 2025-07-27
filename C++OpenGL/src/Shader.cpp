#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filePath)
{
	m_FilePath = filePath;
	ShaderProgramSource source = parseShader(m_FilePath);
	m_programeID = CreateShader(source.vertexSource, source.fragmentSource);
	if (m_programeID == 0) {
		LOG(LogLevel::LOG_LEVEL_ERROR, "着色器创建失败: " + m_FilePath);
		return;
	}
	LOG(LogLevel::LOG_LEVEL_INFO, "着色器创建完成: " + m_FilePath);
    LOG(LogLevel::LOG_LEVEL_INFO, "顶点着色器源程序: \n" + source.vertexSource);
	LOG(LogLevel::LOG_LEVEL_INFO, "片元着色器源程序: \n" + source.fragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_programeID);
}

void Shader::bind() const
{
	glUseProgram(m_programeID);
	// 暂时禁言，每帧都在狗叫有点烦
	//LOG(LogLevel::LOG_LEVEL_INFO, "Shader '" + m_FilePath + "' bound successfully.");   
}

void Shader::unbind() const
{
	glUseProgram(0);
	LOG(LogLevel::LOG_LEVEL_INFO, "着色器解绑成功: " + m_FilePath);
}

Shader::ShaderProgramSource Shader::parseShader(const std::string& filePath) {

   std::ifstream stream(filePath);

   enum class ShaderType {
       NONE = -1,
       VERTEX,
       FRAGMENT
   };

   ShaderType type = ShaderType::NONE;
   std::string line;
   std::stringstream ss[2];
   while (std::getline(stream, line)) {
       if (line.find("#shader") != std::string::npos) {
           if (line.find("vertex") != std::string::npos)
               type = ShaderType::VERTEX;
           else if (line.find("fragment") != std::string::npos)
               type = ShaderType::FRAGMENT;
       }
       else {
           ss[(int)type] << line << '\n';
       }
   }

   return { ss[0].str(), ss[1].str() };
}

void Shader::setUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // 错误处理
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
		// 输出错误信息
        if(type == GL_VERTEX_SHADER) {
            LOG(LogLevel::LOG_LEVEL_ERROR, "顶点着色器编译失败: " + std::string(message));
        } else if(type == GL_FRAGMENT_SHADER) {
            LOG(LogLevel::LOG_LEVEL_ERROR, "片元着色器编译失败: " + std::string(message));
		}
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int Shader::getUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	int location = glGetUniformLocation(m_programeID, name.c_str());
	m_UniformLocationCache[name] = location;
	if (location == -1) {
		LOG(LogLevel::LOG_LEVEL_ERROR, "统一变量 '" + name + "' 不存在于着色器: " + m_FilePath);
	}
	return location;
}
