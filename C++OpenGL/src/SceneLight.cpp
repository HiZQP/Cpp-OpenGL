#include "SceneLight.h"

SceneLightManager::SceneLightManager()
{
	m_DirectionalLightUBO = createUniformBuffer(0, (MAX_LIGHTS + 1) * sizeof(DirectionalLight));
	if (m_DirectionalLightUBO == 0) {
		LOG(LogLevel::LOG_LEVEL_ERROR, "创建方向光源统一缓冲区失败");
		return;
	}
}

SceneLightManager::~SceneLightManager()
{
}

void SceneLightManager::addLight(DirectionalLight light)
{
	if (m_DirectionalLights.size() >= MAX_LIGHTS) {
		LOG(LogLevel::LOG_LEVEL_ERROR, "无法添加更多光源，已达到最大限制");
		return;
	}
	m_DirectionalLights.push_back(light);
	updateDirectionalLights();
}



void SceneLightManager::updateDirectionalLights()
{
	if (!m_DirectionalLights.empty()) {
		int count = m_DirectionalLights.size();
		updateUniformBuffer(m_DirectionalLightUBO, 0, sizeof(int), (const void*)&count);
		updateUniformBuffer(m_DirectionalLightUBO, 16, m_DirectionalLights.size() * sizeof(DirectionalLight), m_DirectionalLights.data());
	}
}

