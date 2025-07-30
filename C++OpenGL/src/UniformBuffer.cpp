#include "UniformBuffer.h"

unsigned int createUniformBuffer(unsigned int bindingPoint, unsigned int size){
	unsigned int ubo;
	GLCall(glCreateBuffers(1, &ubo));
	GLCall(glNamedBufferStorage(ubo, size, nullptr, GL_DYNAMIC_STORAGE_BIT));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo));
	return ubo;
}

void updateUniformBuffer(const unsigned int ubo, const unsigned int offset, const unsigned int size, const void* data) {
	GLCall(glNamedBufferSubData(ubo, offset, size, data));
}
