#include "pch.h"
#include "SceneGraph.h"
#include "SDL.h"
#include "CatchInput.h"


SceneGraph::SceneGraph()
	: m_CurrentSceneIdx{}
{

}

void SceneGraph::ProcessInput(float elapsedSec)
{
	if (m_pScenes.empty()) return;

	switch (CatchInput::GetAction())
	{
	case Action::ToggleScene:
		++m_CurrentSceneIdx;
		if (m_CurrentSceneIdx >= m_pScenes.size()) m_CurrentSceneIdx = 0; // Set current scene back to first element
		break;
	}

	m_pScenes[m_CurrentSceneIdx]->ProcessInput(elapsedSec);
}

void SceneGraph::Update(float elapsedSec)
{
	m_pScenes[m_CurrentSceneIdx]->Update(elapsedSec);
}

int SceneGraph::GetAmountOfScenes() const
{
	return int(m_pScenes.size());
}

Scene* SceneGraph::operator[](int idx)
{
	if (idx >= m_pScenes.size()) return m_pScenes[m_pScenes.size() - 1]; // return last scene-element

	return m_pScenes[idx];
}

Scene* SceneGraph::GetCurrentScene() 
{
	return m_pScenes[m_CurrentSceneIdx];
}

void SceneGraph::AddScene(Scene* scene)
{
	m_pScenes.emplace_back(scene);
}