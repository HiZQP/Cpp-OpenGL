#pragma once

#include <vector>

class SceneEditor
{
public:
	static SceneEditor& getInstance();

	SceneEditor(const SceneEditor&) = delete;// ��ֹ�������캯��

	SceneEditor& operator=(const SceneEditor&) = delete; // ��ֹ��ֵ������



private:
	SceneEditor();// ˽�й��캯������ֹ�ⲿʵ����
	~SceneEditor();
};

