#pragma once

#include "glm/glm.hpp"
#include "UniformBuffer.h"

#define MAX_LIGHTS 32 // 最大光源数量

//struct DirectionalLightData {
//	glm::vec4 ambient; // 环境光
//	glm::vec4 diffuse; // 漫反射光
//	glm::vec4 specular; // 镜面反射光
//	glm::vec4 direction; // 光源方向
//};
//
//struct PointLightData {
//	glm::vec4 ambient; // 环境光
//	glm::vec4 diffuse; // 漫反射光
//	glm::vec4 specular; // 镜面反射光
//	glm::vec4 position; // 光源位置
//	float constant; // 常数衰减系数
//	float linear; // 线性衰减系数
//	float quadratic; // 二次衰减系数
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
//	virtual void update() = 0; // 更新光源数据，子类实现
//	glm::vec4 m_Ambient; // 环境光
//	glm::vec4 m_Diffuse; // 漫反射光
//	glm::vec4 m_Specular; // 镜面反射光
//	bool m_Enabled; // 是否启用光源
//	unsigned int m_RendererID; // 渲染器ID，用于OpenGL渲染
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
//	glm::vec4 m_Direction; // 光源方向
//};

struct DirectionalLight {
	glm::vec4 color; // 光源颜色
	glm::vec4 direction; // 光源方向
};// 占用 32 字节

struct PointLight {
	glm::vec4 color; // 光源颜色
	glm::vec4 position; // 光源位置
	float constant; // 常数衰减系数
	float linear; // 线性衰减系数
	float quadratic; // 二次衰减系数
	float padding; // 填充以对齐到16字节
};// 占用 48 字节

//struct DirectionalLightUBO {
//	int lightCount;                      // 4字节
//	char _padding1[12];                 // 填充到 16 字节（因为数组按 vec4 对齐）
//	DirectionalLight lights[MAX_LIGHTS]; // 每个 32 字节
//};

//struct PointLightUBO {
//	int lightCount;                      // 4字节
//	char _padding2[12];                 // 填充到 16 字节
//	PointLight lights[MAX_LIGHTS];       // 每个 48 字节
//};

// 管理场景中的光源，所添加的光源是静态的。
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
