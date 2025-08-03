#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

// �¼�����
enum class EventType{
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased,
	MouseMoved, MouseScrolled
};

// �¼�����
class Event {
public:
	virtual ~Event() = default;
	virtual EventType getType() const = 0;
};

/*
�ӿ�
*/
class IEventHandler {
public:
	virtual ~IEventHandler() = default;
	virtual void handleEvent(const Event& event) = 0;
};

// ��Ҫ�������ж��¼����Ͳ����ö�Ӧ�Ļص�����
template<typename T>
class EventHandler : public IEventHandler {
public:

	explicit EventHandler(std::function<void(const T&)> handler)
		: thisFunc(handler) { }

	void handleEvent(const Event& event) override {
		if (const T * thisEvent = dynamic_cast<const T*>(&event)) {
			// �ص���������
			thisFunc(*thisEvent);
		}
	}
private:
	// ����Ļص�����
	std::function<void(const T&)> thisFunc;
};

/*
�¼������࣬����ʱ��Ķ��ġ�ȡ���ͷַ�
*/
class EventPublisher {
public:
	// ����Ķ���ʵ�����Ƕ����¼�����
	template<typename T>
	void subscribe(std::function<void(const T&)> handler) {
		EventType type = T::getStaticType();
		auto handlerPtr = std::make_shared<EventHandler<T>>(handler);
		handlers[type].push_back(handlerPtr);
	}

	// ȡ�����Ļ�ɾ����Ӧ�¼����͵����д�����
	template<typename T>
	void unsubscribe() {
		EventType type = T().getType();
		handlers.erase(type);
	}

	void dispatch(const Event& event) {
		EventType type = event.getType();
		auto it = handlers.find(type);
		if (it != handlers.end()) {
			for (const auto& handler : it->second) {
				handler->handleEvent(event);
			}
		}
	}

private:
	std::unordered_map<EventType, std::vector<std::shared_ptr<IEventHandler>>> handlers;
};