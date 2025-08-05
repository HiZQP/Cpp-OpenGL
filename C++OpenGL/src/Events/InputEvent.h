#pragma once

#include "Event.h"
#include "Input/InputCode.h"

class KeyEvent : public Event {
public:
	enum class KeyAction { Unknown = -1, Release, Press, Repeat };

	KeyEvent(const KeyCode& key, const KeyAction& action)
		:m_Key(key), m_KeyAction(action){ }

	EventType getType() const override { return EventType::KeyPressed; }
	static EventType getStaticType() { return EventType::KeyPressed; }

	inline int getKeyInt() const { return (int)m_Key; }
	inline KeyCode getKey() const { return m_Key; }
	inline int getKeyActionInt() const { return (int)m_KeyAction; }
	inline KeyAction getKeyAction() const { return m_KeyAction; }

private:
	KeyCode m_Key;
	KeyAction m_KeyAction;
};

//class KeyReleasedEvent : public Event {
//public:
//	KeyReleasedEvent(const int& key)
//		:m_Key(key){ }
//
//	EventType getType() const override { return EventType::KeyReleased; }
//
//private:
//	int m_Key;
//};

class MouseButtonEvent : public Event {
public:
	enum class MouseButtonAction { Unknown = -1, Release, Press, Repeat };

	MouseButtonEvent(const MouseCode& mouseButton, const MouseButtonAction& action)
		:m_MouseButton(mouseButton), m_MouseButtonAction(action){ }

	EventType getType() const override { return EventType::MouseButtonPressed; }
	static EventType getStaticType() { return EventType::MouseButtonPressed; }

	inline int getMouseButtonInt() const { return (int)m_MouseButton; }
	inline MouseCode getMouseButton() const { return m_MouseButton; }
	inline int getMouseButtonActionInt() const { return (int)m_MouseButtonAction; }
	inline MouseButtonAction getMouseButtonAction() const { return m_MouseButtonAction; }

private:
	MouseCode m_MouseButton;
	MouseButtonAction m_MouseButtonAction;
};

class MouseMovedEvent : public Event {
	public:
	MouseMovedEvent(double xPos, double yPos)
		:m_XPos(xPos), m_YPos(yPos) { }

	EventType getType() const override { return EventType::MouseMoved; }
	static EventType getStaticType() { return EventType::MouseMoved; }

	inline double getXPos() const { return m_XPos; }
	inline double getYPos() const { return m_YPos; }
	inline glm::vec2 getPosition() const { return glm::vec2(m_XPos, m_YPos); }

	glm::vec3 getMouseOffset() const {
		static double lastX = m_XPos, lastY = m_YPos;
		double deltaX = m_XPos - lastX;
		double deltaY = lastY - m_YPos;
		lastX = m_XPos;
		lastY = m_YPos;
		return glm::vec3(deltaX, deltaY, 0.0f);
	}

private:
	double m_XPos;
	double m_YPos;
};

//class MouseButtonReleasedEvent : public Event {
//public:
//
//	MouseButtonReleasedEvent(int mouseButton)
//		:m_MouseButton(mouseButton) {
//	}
//
//	EventType getType() const override { return EventType::MouseButtonPressed; }
//
//private:
//	int m_MouseButton;
//};