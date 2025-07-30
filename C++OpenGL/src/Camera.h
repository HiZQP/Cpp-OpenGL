#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	Camera(const glm::vec3& pos, const glm::vec3& rotation, float aspectRatio, float fov, float nearPlane, float farPlane);
	~Camera() = default;
	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;
	void move(const glm::vec3& direction);
	void rotate(const glm::vec3& rotation);

	inline void setSpeed(float speed) { m_Speed = speed; }
	inline void setMouseSensitivity(float sensitivity) { m_MouseSensitivity = sensitivity; }
	//����FOV
	inline void setFov(float fov) { m_Fov = fov; }

	inline const glm::vec3& getPos() const { return m_Position; }
	inline const glm::vec3& getRot() const { return m_Rotation; }
	inline const glm::vec3& getFront() const { return m_Front; }
	inline float getFov() const { return m_Fov; }

private:
	void updateCameraFront();
private:
	glm::vec3 m_Position; // ���λ��
	glm::vec3 m_Rotation; // �����ת�Ƕȣ�ŷ���ǣ�
	glm::vec3 m_Front; // ���ǰ��
	glm::vec3 m_Right; // �������
	glm::vec3 m_Up; // ����Ϸ���
	glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f); // �����Ϸ���

	float m_Fov; // ��Ұ��Χ
	float m_AspectRatio; // �ݺ��
	float m_NearPlane; // ���ü���
	float m_FarPlane; // Զ�ü���

	float m_Speed = 0.1f; // ����ƶ��ٶ�
	float m_MouseSensitivity = 0.2f;
};

