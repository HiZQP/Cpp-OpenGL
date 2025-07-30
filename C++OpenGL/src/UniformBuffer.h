#pragma once

#include "Renderer.h"

unsigned int createUniformBuffer(unsigned int bindingPoint, unsigned int size);

void updateUniformBuffer(unsigned int ubo, unsigned int offset, unsigned int size, const void* data);