#include "pch.h"
#include "PTriangle.h"
#include "Ray.h"
#include "HitInfo.h"
#include <iostream>

PTriangle::PTriangle(const std::vector<FPoint3>& points, Material* pMaterial, bool isVisible, Cullmode cullMode)
	: PTriangle(points[0], points[1], points[2], pMaterial, isVisible, cullMode)
{

}

PTriangle::PTriangle(const FPoint3& p0, const FPoint3& p1, const FPoint3& p2, Material* pMaterial, bool isVisible, Cullmode cullMode)
	: Primitive(FPoint3{ (p0.x + p1.x + p2.x) / 3.f, (p0.y + p1.y + p2.y) / 3.f, (p0.z, p1.z, p2.z) / 3.f }, pMaterial, isVisible)
	, m_Cullmode{cullMode}
{
	m_Abs_Points.push_back(p0);
	m_Abs_Points.push_back(p1);
	m_Abs_Points.push_back(p2);

	FVector3 side0{ m_Abs_Points[1] - m_Abs_Points[0] };
	FVector3 side1{ m_Abs_Points[2] - m_Abs_Points[0] };

	// PP cross example tricks you!

	m_Normal = Elite::Cross(side0, side1); 

	Elite::Normalize(m_Normal);
}

PTriangle::PTriangle(const FPoint3& origin, std::vector<FPoint3> rel_Points, Material* pMaterial, bool isVisible, Cullmode cullMode)
	: PTriangle(origin, rel_Points[0], rel_Points[1], rel_Points[2], pMaterial, isVisible, cullMode)
{

}

PTriangle::PTriangle(const FPoint3& origin, const FPoint3& rel_P0, const FPoint3& rel_P1, const FPoint3& rel_P2, Material* pMaterial, bool isVisible, Cullmode cullMode)
	: Primitive(origin, pMaterial, isVisible)
	, m_Cullmode{cullMode}
{
	// No '+' operator for points
	m_Abs_Points.push_back(origin + FVector3{ rel_P0 });
	m_Abs_Points.push_back(origin + FVector3{ rel_P1 });
	m_Abs_Points.push_back(origin + FVector3{ rel_P2 });

	FVector3 side0{ m_Abs_Points[1] - m_Abs_Points[0] };
	FVector3 side1{ m_Abs_Points[2] - m_Abs_Points[0] };

	// PP cross example tricks you!

	m_Normal = Elite::Cross(side0, side1); 

	Elite::Normalize(m_Normal);
}

bool PTriangle::Hit(const Ray& ray, HitInfo& hitInfo) const
{
	return StaticHit(ray, hitInfo, m_Normal, m_Abs_Points, m_Cullmode);
}

Cullmode PTriangle::GetCullMode() const
{
	return m_Cullmode;
}

void PTriangle::Rotate(float x, float y, float z)
{
	// calc relative points
	FVector3 originToP1{m_Abs_Points[1] - m_Origin};
	FVector3 originToP2{m_Abs_Points[2] - m_Origin};
	FVector3 originToP0{m_Abs_Points[0] - m_Origin};

	// Rotate relative points
	FMatrix3 points{originToP0, originToP1, originToP2};
	FMatrix3 rot = Elite::MakeRotationZYX(x, y, z);

	points = rot * points;

	// re-calc absolute points
	for (int i{}; i < 3; ++i)
	{
		m_Abs_Points[i] = m_Origin + points[i];
	}

	FVector3 side0{ m_Abs_Points[1] - m_Abs_Points[0] };
	FVector3 side1{ m_Abs_Points[2] - m_Abs_Points[0] };

	m_Normal = Elite::Cross(side0, side1); // Slide example tricks you!!

	Elite::Normalize(m_Normal);
}

void PTriangle::RotateAroundPoint(float x, float y, float z, const FPoint3& pivotPoint)
{

}

bool PTriangle::StaticHit(const Ray& ray, HitInfo& hitInfo, const FVector3& normal, const std::vector<FPoint3>& points, Cullmode cullmode)
{
	if (Elite::AreEqual(Elite::Dot(ray.m_Direction, normal), 0.f)) return false;

	int toggle{};
	if (ray.m_Type == Raytype::Shadow) toggle = -1;
	else toggle = 1;

	// Check cullmode
	switch (cullmode)
	{
	case Cullmode::Frontface:
		if (Elite::Dot(toggle * normal, ray.m_Direction) < 0.f) return false;
		break;

	case Cullmode::Backface:
		if (Elite::Dot(toggle * normal, ray.m_Direction) > 0.f) return false;
		break;
	}

	// Calc triangle-origin
	FPoint3 origin = FPoint3{ (FVector3{ points[0] } + FVector3{ points[1] } + FVector3{ points[2] }) / 3.f };

	// Hits on triangleplane
	float t = Elite::Dot(origin - ray.m_Origin, normal) / Elite::Dot(ray.m_Direction, normal);

	if (t < ray.m_Min || t >= ray.m_Max)
	{
		return false;
	}

	// Hits on actual triangle?
	FPoint3 hitPoint{ ray.m_Origin + t * ray.m_Direction };

	FVector3 pointToCenter{};
	FVector3 pointToNextPoint{};

	// Dot(normal, cross) > 0 --> Same direction --> 1/3 checked well, else return false
	// Check this 3 times without 1 false
	for (int i{}; i < 3; ++i)
	{
		pointToCenter = hitPoint - points[i];
		pointToNextPoint = points[(i + 1) % 3] - points[i];

		float dot = Elite::Dot(normal, Elite::Cross(pointToNextPoint, pointToCenter));
		// negative --> Looks away
		// positive --> looks to cam

		if (Elite::AreEqual(dot, 0.f)) return false;

		if (dot < 0)
		{
			return false;
		}
	}

	hitInfo.m_T = t;
	hitInfo.m_HitPoint = hitPoint;
	hitInfo.m_Normal = normal;

	return true;
}