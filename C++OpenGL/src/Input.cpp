#include "Input.h"

bool Input::isKeyPressed(int key) const
{
	return glfwGetKey(m_window, key) == GLFW_PRESS;
}
bool Input::isMouseButtonPressed(int button) const
{
	return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

glm::vec2 Input::getMousePosition() const
{
	double x, y;
	glfwGetCursorPos(m_window, &x, &y);
	return glm::vec2(x, y);
}

glm::vec3 Input::getMouseAcceleration() const
{
	double x, y;
	glfwGetCursorPos(m_window, &x, &y);
	static double lastX = x, lastY = y;
	double deltaX = x - lastX;
	double deltaY = lastY -y;
	lastX = x;
	lastY = y;
	return glm::vec3(deltaX, deltaY, 0.0f);
}