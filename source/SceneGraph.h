#pragma once
#include <vector>
#include <list>
#include <memory>
#include "Ray.h"
#include "HitInfo.h"
#include "Scene.h"

class Primitive;
class PSphere;
class PPlane;
class Light;
class LDirection;
class LPoint;
class Camera;
class PTriangle;
class PMesh;

class SceneGraph
{
public:
	static SceneGraph& Instance()
	{
		// Automatic destruction + first time instatiated
		static SceneGraph instance;
		return instance;
	}

	SceneGraph(const SceneGraph& other) = delete;
	SceneGraph(SceneGraph&& other) = delete;
	void operator=(const SceneGraph& rhs) = delete;
	void operator=(SceneGraph&& rhs) = delete;

	void Update(float elapsedSec);
	void ProcessInput(float elapsedSec);
	int GetAmountOfScenes() const;

	Scene* operator[](int idx);
	Scene* GetCurrentScene();
	void AddScene(Scene* scene);
	
private:
	SceneGraph();
	std::vector<Scene*> m_pScenes;
	int m_CurrentSceneIdx;
};
