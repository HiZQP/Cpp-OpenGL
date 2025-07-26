#include "Entity.h"

glm::mat4 Entity::getWorldTransform() const
{
	glm::mat4 worldTransform = m_Transform.toMat4();
	Entity* parent = m_Parent;
	while (parent)
	{
		worldTransform = parent->getTransform().toMat4() * worldTransform;
		parent = parent->m_Parent;
	}
	return worldTransform;
}

void Entity::addChild(std::shared_ptr<Entity> child)
{
	if (child)
	{
		child->m_Parent = this;
		m_Children.push_back(child);
	}
}