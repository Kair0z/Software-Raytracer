#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Material.h"

struct Ray;
struct HitInfo;

using namespace Elite;

class Primitive
{
public:
	Primitive(const FPoint3& origin, Material* pMaterial, bool isVisible = true);
	
	// Hit function
	virtual bool Hit(const Ray& ray, HitInfo& hitInfo) const = 0; 
	virtual void Rotate(float x, float y, float z) = 0;
	const Material* GetMaterial() const;

	bool IsVisible() const;

	~Primitive()
	{
		delete m_pMaterial;
		m_pMaterial = nullptr;
	}

protected:
	bool m_IsVisible;
	Elite::FPoint3 m_Origin;
	Material* m_pMaterial;
};