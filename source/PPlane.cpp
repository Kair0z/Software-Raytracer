#include "pch.h"
#include "PPlane.h"
#include "HitInfo.h"
#include "Ray.h"
#include <iostream>



PPlane::PPlane(const FPoint3& origin, const FVector3& normal, Material* pMaterial, bool isVisible)
	: Primitive(origin, pMaterial, isVisible)
	, m_Normal{normal}
{

}

bool PPlane::Hit(const Ray& ray, HitInfo& hitInfo) const
{
	// find t
	float t = Elite::Dot(m_Origin - ray.m_Origin, m_Normal) / Elite::Dot(ray.m_Direction, m_Normal);

	if (t < ray.m_Min || t >= ray.m_Max)
	{
		return false;
	}

	hitInfo.m_T = t;
	hitInfo.m_HitPoint = ray.m_Origin + t * ray.m_Direction;
	hitInfo.m_Normal = m_Normal;
	hitInfo.m_pMaterial = m_pMaterial;

	return true;
}

void PPlane::Rotate(float x, float y, float z)
{
	m_Normal = Elite::MakeRotationZYX(x, y, z) * m_Normal;
}