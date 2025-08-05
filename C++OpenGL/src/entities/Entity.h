#pragma once

#include <vector>
#include <memory>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"


struct Transform
{
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	glm::mat4 toMat4() const
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
		rotation = glm::rotate(rotation, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
		rotation = glm::rotate(rotation, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);
		return translation * rotation * scale;
	}
};

class Entity
{
public:
	Entity() = default;
	virtual ~Entity() = default;

	virtual void update(float deltaTime) = 0;
	virtual void render() const {}
	virtual bool isAlive() const { return true; }
	virtual void destroy() {}
	inline const Transform& getTransform() const { return m_Transform; }
	
	glm::mat4 getWorldTransform() const;
	void setLocalTransform(const Transform& transform) { m_Transform = transform; }
	void addChild(std::shared_ptr<Entity> child);

private:
	Transform m_Transform; // 实体变换
	std::vector<std::shared_ptr<Entity>> m_Children; // 子实体列表
	Entity* m_Parent = nullptr; // 父实体指针
	unsigned int m_ID; // 实体ID
};

