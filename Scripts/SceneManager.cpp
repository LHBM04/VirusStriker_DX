#pragma once

#include <map>
#include <string>
#include <memory>
#include <stack>

#include "Singleton.h"
#include "Scene.h"

class Scene;
class SceneManager : public Singleton<SceneManager> {
private:
	std::map<std::wstring, std::unique_ptr<Scene>> _scene;
	std::stack<std::unique_ptr<Scene>> _previousScenes;
};