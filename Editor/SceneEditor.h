#pragma once

#include <vector>

class SceneEditor
{
public:
	static SceneEditor& getInstance();

	SceneEditor(const SceneEditor&) = delete;// 禁止拷贝构造函数

	SceneEditor& operator=(const SceneEditor&) = delete; // 禁止赋值操作符



private:
	SceneEditor();// 私有构造函数，禁止外部实例化
	~SceneEditor();
};

