#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

// 事件类型
enum class EventType{
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased,
	MouseMoved, MouseScrolled
};

// 事件基类
class Event {
public:
	virtual ~Event() = default;
	virtual EventType getType() const = 0;
};

/*
接口
*/
class IEventHandler {
public:
	virtual ~IEventHandler() = default;
	virtual void handleEvent(const Event& event) = 0;
};

// 主要功能是判断事件类型并调用对应的回调函数
template<typename T>
class EventHandler : public IEventHandler {
public:

	explicit EventHandler(std::function<void(const T&)> handler)
		: thisFunc(handler) { }

	void handleEvent(const Event& event) override {
		if (const T * thisEvent = dynamic_cast<const T*>(&event)) {
			// 回调函数处理
			thisFunc(*thisEvent);
		}
	}
private:
	// 储存的回调函数
	std::function<void(const T&)> thisFunc;
};

/*
事件管理类，负责时间的订阅、取消和分发
*/
class EventPublisher {
public:
	// 这里的订阅实际上是订阅事件类型
	template<typename T>
	void subscribe(std::function<void(const T&)> handler) {
		EventType type = T::getStaticType();
		auto handlerPtr = std::make_shared<EventHandler<T>>(handler);
		handlers[type].push_back(handlerPtr);
	}

	// 取消订阅会删除对应事件类型的所有处理器
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