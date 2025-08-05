#pragma once

#include "Input/InputCode.h"
#include "Entities/Entity.h"

// ¿ØÖÆÆ÷»ùÀà
class Controller {
public:
	Controller(const std::shared_ptr<Entity>& entity) 
		: m_Entity(entity) {
	}
	virtual ~Controller() = 0;

private:
	std::shared_ptr<Entity> m_Entity;
};