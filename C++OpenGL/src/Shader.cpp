#include "Shader.h"

Shader::Shader(const std::string& filePath)
{
	m_FilePath = filePath;
	ShaderProgramSource source = parseShader(m_FilePath);
	m_programeID = CreateShader(source.vertexSource, source.fragmentSource);
	if (m_programeID == 0) {
		LOG(LogLevel::LOG_LEVEL_FATAL, "Failed to create shader program from file: " + m_FilePath);
		return;
	}
	LOG(LogLevel::LOG_LEVEL_INFO, "Shader program created successfully from file: " + m_FilePath);
    LOG(LogLevel::LOG_LEVEL_INFO, "Vertex Shader Source : \n" + source.vertexSource);
	LOG(LogLevel::LOG_LEVEL_INFO, "Fragment Shader Source : \n" + source.fragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_programeID);
}

void Shader::bind() const
{
	glUseProgram(m_programeID);
	LOG(LogLevel::LOG_LEVEL_INFO, "Shader '" + m_FilePath + "' bound successfully.");   
}

void Shader::unbind() const
{
	glUseProgram(0);
	LOG(LogLevel::LOG_LEVEL_INFO, "Shader '" + m_FilePath + "' unbound successfully.");
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

}

 unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // ������
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
		// ���������Ϣ
        std::cout << "Failed compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
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

GLint Shader::getUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	GLint location = glGetUniformLocation(m_programeID, name.c_str());
	m_UniformLocationCache[name] = location;
	if (location == -1) {
		LOG(LogLevel::LOG_LEVEL_ERROR, "Uniform '" + name + "' doesn't exist in shader '" + m_FilePath + "'");
	}
	return location;
}
