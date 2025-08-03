#include "Input.h"

Input& Input::getInstance(){
	static Input instance;
	return instance;
}

void Input::init(GLFWwindow* window, std::shared_ptr<EventPublisher> eventPublisher){
	m_Window = window;
	m_EventPublisher = std::move(eventPublisher);
	glfwSetKeyCallback(m_Window, &Input::updateKeyInput);
}

bool Input::isKeyPressed(int key) const{
	return glfwGetKey(m_Window, key) == GLFW_PRESS;
}

bool Input::isMouseButtonPressed(int button) const{
	return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
}

glm::vec2 Input::getMousePosition() const{
	double x, y;
	glfwGetCursorPos(m_Window, &x, &y);
	return glm::vec2(x, y);
}

glm::vec3 Input::getMouseOffset() const{
	double x, y;
	glfwGetCursorPos(m_Window, &x, &y);
	static double lastX = x, lastY = y;
	double deltaX = x - lastX;
	double deltaY = lastY -y;
	lastX = x;
	lastY = y;
	return glm::vec3(deltaX, deltaY, 0.0f);
}

void Input::updateRawInputInLoop()
{
	glfwSetKeyCallback(m_Window, &Input::updateKeyInput);
	glfwSetMouseButtonCallback(m_Window, &Input::updateMouseButtonInput);
}

void Input::updateKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyEvent keyEvent(key, (KeyEvent::KeyAction)action);
	Input::getInstance().m_EventPublisher->dispatch(keyEvent);
}

void Input::updateMouseButtonInput(GLFWwindow* window, int button, int action, int mods)
{
	MouseButtonEvent mouseButtonEvent(button, (MouseButtonEvent::MouseButtonAction)action);
	Input::getInstance().m_EventPublisher->dispatch(mouseButtonEvent);
}

void Input::updateMousePositionInput(GLFWwindow* window, double xpos, double ypos)
{
	MouseMovedEvent mouseMovedEvent(xpos, ypos);
	Input::getInstance().m_EventPublisher->dispatch(mouseMovedEvent);
}

Input::Input()
	: m_Window(nullptr), m_EventPublisher(nullptr) {
}
Input::~Input() {
}
