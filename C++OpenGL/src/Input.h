#pragma once

#include "Renderer.h"

class Input
{
public:
	Input(GLFWwindow* window) : m_window(window) {}

	bool isKeyPressed(int key) const;
	bool isMouseButtonPressed(int button) const;
	glm::vec2 getMousePosition() const;
	glm::vec3 getMouseAcceleration() const;

private:
	GLFWwindow* m_window;
};

