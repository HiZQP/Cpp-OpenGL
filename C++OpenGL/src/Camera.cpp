#include "Camera.h"

Camera::Camera(const glm::vec3& pos, const glm::vec3& rotation, float aspectRatio, float fov, float nearPlane, float farPlane)
	: m_Position(pos), m_Rotation(rotation), m_AspectRatio(aspectRatio), m_Fov(fov), m_NearPlane(nearPlane), m_FarPlane(farPlane)
{
	updateCameraFront();
}

glm::mat4 Camera::getProjectionMatrix() const
{
	glm::mat4 projection = glm::perspective(
		glm::radians(m_Fov), // ��Ұ��Χ
		m_AspectRatio, // �ݺ��
		m_NearPlane, // ���ü���
		m_FarPlane // Զ�ü���
	);
	return projection;
}

glm::mat4 Camera::getViewMatrix() const
{
	glm::mat4 view = glm::lookAt(
		m_Position, // ���λ��
		m_Position + m_Front, // �������
		m_WorldUp // �Ϸ���
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
	// ������ת�Ƕ�
	m_Rotation.x = glm::clamp(m_Rotation.x, -89.0f, 89.0f); // ���Ƹ����Ƕ�
	m_Rotation.y = glm::mod(m_Rotation.y, 360.0f); // ����ƫ���Ƕ���0-360��֮��
	updateCameraFront();
}

void Camera::updateCameraFront()
{
	glm::vec3 front;
	// ʹ��ŷ���Ǽ���ǰ������ x-pitch y-yaw z-row
	front.x = cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
	front.y = sin(glm::radians(m_Rotation.x));
	front.z = sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
	// ��һ��ǰ������
	m_Front = glm::normalize(front);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Front, m_Right));
}

