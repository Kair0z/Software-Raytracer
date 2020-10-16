#include "pch.h"
#include "Scene.h"
#include "PSphere.h"
#include "PPlane.h"
#include "LDirection.h"
#include "LPoint.h"
#include "Camera.h"
#include <iostream>
#include <iomanip>
#include "SDL.h"
#include "PTriangle.h"
#include "PMesh.h"
#include "CatchInput.h"

Scene::Scene()
	: m_ActiveCameraIdx{}
{

}

Scene::~Scene()
{
	Cleanup();
}

void Scene::Cleanup()
{
	for (Primitive* pElement : m_pElements)
	{
		delete pElement;
		pElement = nullptr;
	}

	for (Light* pLight : m_pLights)
	{
		delete pLight;
		pLight = nullptr;
	}

	for (Camera* pCamera : m_pCameras)
	{
		delete pCamera;
		pCamera = nullptr;
	}
}

void Scene::Update(float elapsedSec)
{
	for (Primitive* pElement : m_pElements)
	{
		if (dynamic_cast<PTriangle*>(pElement) != nullptr)
		{
			dynamic_cast<PTriangle*>(pElement)->Rotate(0.f, elapsedSec, 0.f);
		}

		PMesh* mesh = dynamic_cast<PMesh*>(pElement);
		if (mesh != nullptr)
		{
			mesh->Rotate(0.f, elapsedSec, 0.f);
		}
	}

	for (Light* pLight : m_pLights)
	{
		pLight->Update(elapsedSec);
	}
}

bool Scene::HasCamera() const
{
	return bool(m_pCameras.size());
}

void Scene::ProcessInput(float elapsedSec)
{
	switch (CatchInput::GetAction())
	{
	case Action::ToggleCamera:
		++m_ActiveCameraIdx;
		if (m_ActiveCameraIdx >= m_pCameras.size()) m_ActiveCameraIdx = 0;
		break;

	case Action::ResetCameras:
		for (Camera* pCam : m_pCameras)
		{
			pCam->ResetPosition();
		}
		break;
	}

	m_pCameras[m_ActiveCameraIdx]->ProcessInput(elapsedSec);
}

void Scene::AddElement(PSphere* pSphere)
{
	m_pElements.emplace_back(pSphere);
}

void Scene::AddElement(PPlane* pPlane)
{
	m_pElements.emplace_back(pPlane);
}

void Scene::AddElement(LPoint* pPointLight)
{
	m_pLights.emplace_back(pPointLight);
}

void Scene::AddElement(LDirection* pDirectionLight)
{
	m_pLights.emplace_back(pDirectionLight);
}

void Scene::AddElement(PTriangle* pTriangle)
{
	m_pElements.emplace_back(pTriangle);
}

void Scene::AddElement(PMesh* pMesh)
{
	m_pElements.emplace_back(pMesh);
}

RGBColor Scene::Shade(const HitInfo& hitInfo, const FVector3& toView, bool hardShadowsOn) const
{
	RGBColor result{};
	RGBColor materialShade{};

	for (Light* pLight : m_pLights)
	{
		if (pLight->IsOn())
		{
			float dot = -Elite::Dot(hitInfo.m_Normal, pLight->GetDirection(hitInfo));

			// Shadows affect endvalue:
			// If Hardshadows on  --> check also if Point in sight, else, go on to next light OR dot < 0
			if (hardShadowsOn && !PointInSight(pLight, hitInfo) || dot < 0)
			{
				// --> If hardshadowed --> Biradiancevalue becomes 0
				// --> If dot < 0 --> Biradiancevalue becomes 0
				dot = 0.f;
			}

			// Biradiance affects endvalue + (lambert cosine law): 
			result += pLight->BiradianceValue(hitInfo) * dot;

			// Material affects endvalue:
			materialShade += hitInfo.m_pMaterial->Shade(hitInfo, pLight->GetDirection(hitInfo), toView);
		}
	}

	result *= materialShade;

	return result;
}

bool Scene::PointInSight(const Light* pLight, const HitInfo& hitInfo) const
{
	// Just so Hit() can input an unused hitinfo (we don't need the point, just need to know if that spot is in shadow)
	HitInfo hitCopy{ hitInfo };

	FVector3 toLightVector{ -pLight->GetDirection(hitCopy) };
	float lightDistance = Elite::Distance(hitCopy.m_HitPoint, pLight->GetPos());

	Ray pointToLight{ hitCopy.m_HitPoint, toLightVector, 0.01f, lightDistance, Raytype::Shadow };

	// If there is a hit --> Point is not in light
	if (Hit(pointToLight, hitCopy) != nullptr)
	{
		return false;
	}

	// If there is no hit --> Point is in light
	else
	{
		return true;
	}
}

Primitive* Scene::Hit(const Ray& ray, HitInfo& hitInfo) const
{
	// order of hit setup
	Primitive* closestObj{ nullptr };
	float closestT{ ray.m_Max };
	HitInfo closestHitInfo{};

	// Process-Hit (check every Primitive in graph)
	for (Primitive* pElement : m_pElements)
	{
		if (pElement->IsVisible())
		{
			if (pElement->Hit(ray, closestHitInfo))
			{
				if (closestHitInfo.m_T < closestT)
				{
					closestObj = pElement;

					hitInfo = closestHitInfo;
					closestT = closestHitInfo.m_T;
				}
			}
		}
	}

	return closestObj;
}

void Scene::AddCamera(Camera* cam, bool isNewActive)
{
	m_pCameras.push_back(cam);

	if (isNewActive || m_pCameras.size() == 1) // if new active OR first camera in the scene
	{
		m_ActiveCameraIdx = int(m_pCameras.size()) - 1; // Idx of last added element
	}
}

const Camera* Scene::GetActiveCam() const
{
	if (m_pCameras.empty()) return nullptr;

	return m_pCameras[m_ActiveCameraIdx];
}