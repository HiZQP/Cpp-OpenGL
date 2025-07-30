#pragma once

#include "glm/glm.hpp"
#include "UniformBuffer.h"

#define MAX_LIGHTS 32 // ����Դ����

//struct DirectionalLightData {
//	glm::vec4 ambient; // ������
//	glm::vec4 diffuse; // �������
//	glm::vec4 specular; // ���淴���
//	glm::vec4 direction; // ��Դ����
//};
//
//struct PointLightData {
//	glm::vec4 ambient; // ������
//	glm::vec4 diffuse; // �������
//	glm::vec4 specular; // ���淴���
//	glm::vec4 position; // ��Դλ��
//	float constant; // ����˥��ϵ��
//	float linear; // ����˥��ϵ��
//	float quadratic; // ����˥��ϵ��
//	float padding;
//};
//
//class Light{
//public:
//	Light(const glm::vec4& ambient = glm::vec4(0.2f),
//		  const glm::vec4& diffuse = glm::vec4(1.0f),
//		  const glm::vec4& specular = glm::vec4(1.0f),
//		  bool enabled = true)
//		: m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_Enabled(enabled) {}
//	virtual ~Light() = default;
//
//	inline glm::vec4 getAmbient() const { return m_Ambient; }
//	inline glm::vec4 getDiffuse() const { return m_Diffuse; }
//	inline glm::vec4 getSpecular() const { return m_Specular; }
//	inline bool isEnabled() const { return m_Enabled; }
//
//	inline void setAmbient(const glm::vec4& ambient) { m_Ambient = ambient; update(); }
//	inline void setDiffuse(const glm::vec4& diffuse) { m_Diffuse = diffuse; update(); }
//	inline void setSpecular(const glm::vec4& specular) { m_Specular = specular; update(); }
//	inline void setEnabled(bool enabled) { m_Enabled = enabled; update(); }
//	
//protected:
//	virtual void update() = 0; // ���¹�Դ���ݣ�����ʵ��
//	glm::vec4 m_Ambient; // ������
//	glm::vec4 m_Diffuse; // �������
//	glm::vec4 m_Specular; // ���淴���
//	bool m_Enabled; // �Ƿ����ù�Դ
//	unsigned int m_RendererID; // ��Ⱦ��ID������OpenGL��Ⱦ
//};
//
//class DirectionalLight : public Light {
//public:
//	DirectionalLight(const glm::vec4& direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f),
//					 const glm::vec4& ambient = glm::vec4(0.2f),
//					 const glm::vec4& diffuse = glm::vec4(1.0f),
//					 const glm::vec4& specular = glm::vec4(1.0f),
//					 bool enabled = true)
//		: Light(ambient, diffuse, specular, enabled), m_Direction(direction) {
//		m_RendererID = createUniformBuffer(0, sizeof(DirectionalLightData));
//	}
//
//	inline glm::vec3 getDirection() const { return m_Direction; }
//	inline void setDirection(const glm::vec4& direction) { m_Direction = direction; update();}
//	inline void setLight(const glm::vec4& direction, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, bool enabled) {
//		m_Direction = direction; m_Ambient = ambient; m_Diffuse = diffuse; m_Specular = specular; m_Enabled = enabled;
//		update();
//	}
//
//protected:
//	void update() override {
//		DirectionalLightData data;
//		data.ambient = m_Ambient;
//		data.diffuse = m_Diffuse;
//		data.specular = m_Specular;
//		data.direction = m_Direction;
//		updateUniformBuffer(m_RendererID, &data, sizeof(DirectionalLightData));
//	}
//	glm::vec4 m_Direction; // ��Դ����
//};

struct DirectionalLight {
	glm::vec4 color; // ��Դ��ɫ
	glm::vec4 direction; // ��Դ����
};// ռ�� 32 �ֽ�

struct PointLight {
	glm::vec4 color; // ��Դ��ɫ
	glm::vec4 position; // ��Դλ��
	float constant; // ����˥��ϵ��
	float linear; // ����˥��ϵ��
	float quadratic; // ����˥��ϵ��
	float padding; // ����Զ��뵽16�ֽ�
};// ռ�� 48 �ֽ�

//struct DirectionalLightUBO {
//	int lightCount;                      // 4�ֽ�
//	char _padding1[12];                 // ��䵽 16 �ֽڣ���Ϊ���鰴 vec4 ���룩
//	DirectionalLight lights[MAX_LIGHTS]; // ÿ�� 32 �ֽ�
//};

//struct PointLightUBO {
//	int lightCount;                      // 4�ֽ�
//	char _padding2[12];                 // ��䵽 16 �ֽ�
//	PointLight lights[MAX_LIGHTS];       // ÿ�� 48 �ֽ�
//};

// �������еĹ�Դ������ӵĹ�Դ�Ǿ�̬�ġ�
class SceneLightManager {
public:
	SceneLightManager();
	~SceneLightManager();

	void addLight(DirectionalLight light);
	//void addLight(PointLight light);

	void updateDirectionalLights();
	//void updatePointLights();

private:
	std::vector<DirectionalLight> m_DirectionalLights;
	//std::vector<PointLight> m_PointLights;
	unsigned int m_DirectionalLightUBO;
	//unsigned int m_PointLightUBO;
};
