#pragma once

#include <chrono>

#include "Models/Models.h"

class RenderControl{
public:
	static RenderControl& getInstance();
	RenderControl(const RenderControl&) = delete; // ½ûÖ¹¿½±´¹¹Ôìº¯Êý
	RenderControl& operator=(const RenderControl&) = delete; // ½ûÖ¹¸³Öµ²Ù×÷·û

	void init(const int& targetFPS);

	void calculateDeltaTime();
	void rendering() const;

	inline void setTergetFPS();

	inline float getDeltaTime() const;

private:
	void renderModels() const;

	int m_TargetFPS;
	float m_Delta_MS;

	std::vector<Model> m_Models;

	RenderControl();
	~RenderControl();
};

