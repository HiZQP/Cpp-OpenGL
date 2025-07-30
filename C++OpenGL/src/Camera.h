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
	//设置FOV
	inline void setFov(float fov) { m_Fov = fov; }

	inline const glm::vec3& getPos() const { return m_Position; }
	inline const glm::vec3& getRot() const { return m_Rotation; }
	inline const glm::vec3& getFront() const { return m_Front; }
	inline float getFov() const { return m_Fov; }

private:
	void updateCameraFront();
private:
	glm::vec3 m_Position; // 相机位置
	glm::vec3 m_Rotation; // 相机旋转角度（欧拉角）
	glm::vec3 m_Front; // 相机前向
	glm::vec3 m_Right; // 相机右向
	glm::vec3 m_Up; // 相机上方向
	glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f); // 世界上方向

	float m_Fov; // 视野范围
	float m_AspectRatio; // 纵横比
	float m_NearPlane; // 近裁剪面
	float m_FarPlane; // 远裁剪面

	float m_Speed = 0.1f; // 相机移动速度
	float m_MouseSensitivity = 0.2f;
};

