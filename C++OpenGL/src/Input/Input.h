#pragma once

#include "Renderer.h"
#include "glm/glm.hpp"
#include "Events/InputEvent.h"

class Input{
public:
	static Input& getInstance();
	Input(const Input&) = delete;// ½ûÖ¹¿½±´¹¹Ôìº¯Êý
	Input& operator=(const Input&) = delete; // ½ûÖ¹¸³Öµ²Ù×÷·û

	void init(GLFWwindow* window, std::shared_ptr<EventPublisher> eventPublisher);

	bool isKeyPressed(int key) const;
	bool isMouseButtonPressed(int button) const;
	glm::vec2 getMousePosition() const;
	glm::vec3 getMouseOffset() const;

	void updateRawInputInLoop();

	static void updateKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void updateMouseButtonInput(GLFWwindow* window, int button, int action, int mods);
	static void updateMousePositionInput(GLFWwindow* window, double xpos, double ypos);

private:
	Input();
	~Input();

	GLFWwindow* m_Window;
	std::shared_ptr<EventPublisher> m_EventPublisher;
};

