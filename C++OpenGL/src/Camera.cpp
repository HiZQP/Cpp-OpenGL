#include "Camera.h"

Camera::Camera(const glm::vec3& pos, const glm::vec3& rotation, float aspectRatio, float fov, float nearPlane, float farPlane)
	: m_Position(pos), m_Rotation(rotation), m_AspectRatio(aspectRatio), m_Fov(fov), m_NearPlane(nearPlane), m_FarPlane(farPlane)
{
	updateCameraFront();
}

glm::mat4 Camera::getProjectionMatrix() const
{
	glm::mat4 projection = glm::perspective(
		glm::radians(m_Fov), // 视野范围
		m_AspectRatio, // 纵横比
		m_NearPlane, // 近裁剪面
		m_FarPlane // 远裁剪面
	);
	return projection;
}

glm::mat4 Camera::getViewMatrix() const
{
	glm::mat4 view = glm::lookAt(
		m_Position, // 相机位置
		m_Position + m_Front, // 相机朝向
		m_WorldUp // 上方向
	);
	return view;
}

void Camera::move(const glm::vec3& direction)
{
	m_Position += direction * m_Speed;
	updateCameraFront();
}

void Camera::rotate(const glm::vec3& rotation)
{
	m_Rotation += rotation;
	// 限制旋转角度
	m_Rotation.x = glm::clamp(m_Rotation.x, -89.0f, 89.0f); // 限制俯仰角度
	m_Rotation.y = glm::mod(m_Rotation.y, 360.0f); // 限制偏航角度在0-360度之间
	updateCameraFront();
}

void Camera::updateCameraFront()
{
	glm::vec3 front;
	// 使用欧拉角计算前向向量 x-pitch y-yaw z-row
	front.x = cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
	front.y = sin(glm::radians(m_Rotation.x));
	front.z = sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
	// 归一化前向向量
	m_Front = glm::normalize(front);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Front, m_Right));
}

