#pragma once

#include <vector>
#include <list>
#include <memory>
#include "Ray.h"
#include "HitInfo.h"

class Primitive;
class PSphere;
class PPlane;
class Light;
class LDirection;
class LPoint;
class Camera;
class PTriangle;
class PMesh;

class Scene
{
public:
	Scene();
	void AddElement(PSphere* pSphere);
	void AddElement(PPlane* pPlane);
	void AddElement(PTriangle* pTriangle);
	void AddElement(LPoint* pPointLight);
	void AddElement(LDirection* pDirectionLight);
	void AddElement(PMesh* pMesh);

	~Scene(); // Calls the Cleanup-method

	void AddCamera(Camera* cam, bool isNewActive = false);
	const Camera* GetActiveCam() const;

	Primitive* Hit(const Ray& ray, HitInfo& hitInfo) const;
	RGBColor Shade(const HitInfo& hitInfo, const FVector3& toView, bool hardShadowsOn) const;

	void ProcessInput(float elapsedSec);
	void Update(float elapsedSec);

	bool HasCamera() const;

private:
	void Cleanup();
	bool PointInSight(const Light* pLight, const HitInfo& hitInfo) const;

	// container with objects (to be rendered or not)
	std::vector<Primitive*> m_pElements;
	std::vector<Light*> m_pLights;

	std::vector<Camera*> m_pCameras;
	int m_ActiveCameraIdx;
};

