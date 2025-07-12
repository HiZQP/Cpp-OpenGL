#include "Renderer.h"


void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		LOG(LogLevel::LOG_LEVEL_FATAL,
			"[OpenGL Error] (" + std::to_string(error) + "): " + function +
			" " + file + ":" + std::to_string(line));
		return false;
	}
	return true;
}