#pragma once

#include "Entities/Entity.h"

class SceneEditor : public Entity
{
public:
	static SceneEditor& getInstance();
	SceneEditor(const SceneEditor&) = delete;// ��ֹ�������캯��
	SceneEditor& operator=(const SceneEditor&) = delete; // ��ֹ��ֵ������



private:

	SceneEditor();// ˽�й��캯������ֹ�ⲿʵ����
	~SceneEditor();
};

