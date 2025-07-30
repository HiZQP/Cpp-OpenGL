#include "SceneLight.h"

SceneLightManager::SceneLightManager()
{
	m_DirectionalLightUBO = createUniformBuffer(0, (MAX_LIGHTS + 1) * sizeof(DirectionalLight));
	if (m_DirectionalLightUBO == 0) {
		LOG(LogLevel::LOG_LEVEL_ERROR, "���������Դͳһ������ʧ��");
		return;
	}
}

SceneLightManager::~SceneLightManager()
{
}

void SceneLightManager::addLight(DirectionalLight light)
{
	if (m_DirectionalLights.size() >= MAX_LIGHTS) {
		LOG(LogLevel::LOG_LEVEL_ERROR, "�޷���Ӹ����Դ���Ѵﵽ�������");
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

