#include "pch.h"
#include "PSphere.h"
#include "Ray.h"
#include "EVector3.h"
#include "HitInfo.h"

PSphere::PSphere(const Elite::FPoint3& origin, float radius, Material* pMaterial, bool isVisible)
	: Primitive(origin, pMaterial, isVisible)
	, m_Radius{ radius }
{
	
}

bool PSphere::Hit(const Ray& ray, HitInfo& hitInfo) const
{
	hitInfo.m_pMaterial = m_pMaterial;
	return StaticHit(m_Origin, m_Radius, ray, hitInfo);
}

void PSphere::Rotate(float x, float y, float z)
{

}

bool PSphere::StaticHit(const FPoint3& origin, float radius, const Ray& ray, HitInfo& hitInfo)
{
	float a{ Elite::Dot(ray.m_Direction, ray.m_Direction) };
	float b{ Elite::Dot((2 * ray.m_Direction), (ray.m_Origin - origin)) };
	float c{ Elite::Dot((ray.m_Origin - origin), (ray.m_Origin - origin)) - (radius * radius) };

	float d{ (b * b) - 4 * a * c };

	if (d < 0 || Elite::AreEqual(d, 0.f)) return false;

	float dSqrt{ sqrtf(d) };

	float t1{ (-b - dSqrt) / 2.f * a };
	float t2;

	if (t1 < ray.m_Min)
	{
		t2 = (-b + dSqrt) / 2.f * a;
	}

	if (t1 > ray.m_Max || t1 < ray.m_Min) return false;

	hitInfo.m_HitPoint = ray.m_Origin + t1 * ray.m_Direction;
	hitInfo.m_Normal = Elite::GetNormalized(hitInfo.m_HitPoint - origin);
	hitInfo.m_T = t1;

	return true;
}