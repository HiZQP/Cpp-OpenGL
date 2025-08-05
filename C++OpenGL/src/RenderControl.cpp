#include "RenderControl.h"

RenderControl& RenderControl::getInstance(){
	// 使用单例模式，确保只有一个 FrameControl 实例
	static RenderControl instance;
	return instance;
}

void RenderControl::init(const int& targetFPS)
{
	m_Delta_MS = 1000 / m_TargetFPS;
}

void RenderControl::calculateDeltaTime()
{
	std::chrono::steady_clock::time_point lastFrameTime;
	lastFrameTime = std::chrono::steady_clock::now();
}

void RenderControl::rendering() const
{
}

inline void RenderControl::setTergetFPS()
{
}

inline float RenderControl::getDeltaTime() const
{
	return 0.0f;
}

void RenderControl::renderModels() const
{
}

RenderControl::RenderControl()
{
}

RenderControl::~RenderControl()
{
}
