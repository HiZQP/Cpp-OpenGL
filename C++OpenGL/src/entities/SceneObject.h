#pragma once

#include "Entity.h"

class SceneObject : public Entity
{
public:
	SceneObject() = default;
	virtual ~SceneObject() = default;
	void update(float deltaTime) override {}
	void render() const override {}
	bool isAlive() const override { return true; }
	void destroy() override {}
private:
	unsigned int m_ID;
};