#include "pch.h"
#include "LPoint.h"

LPoint::LPoint(const Elite::FPoint3& pos, const Elite::RGBColor& color, float intensity, bool isOn)
	: Light(color, intensity, isOn)
	, m_Pos{pos}
{

}

Elite::FVector3 LPoint::GetDirection(const HitInfo& hitInfo) const
{
	return Elite::GetNormalized(Elite::FVector3{ hitInfo.m_HitPoint - m_Pos });;
}

Elite::RGBColor LPoint::BiradianceValue(const HitInfo& hitInfo) const
{
	return m_Color * (m_Intensity / Elite::SqrDistance(m_Pos, hitInfo.m_HitPoint));
	// SqrDistance should be relatively fast --> Dot of the vector between points with itself!

	//--------------------
	// result == color * E
	// I = d(Phi) / d(omg)
	// --> d(Phi) == I * d(omg)
	// --> d(Phi) integrated over a sphere == I * 4PI
	//
	// E == d(Phi) / d(Area)
	// E == d(Phi) / 4PI * sqr(rad) [Area of the surrounding sphere]
	// 
	// Simplified:
	// E == I / sqr(rad)
	// E == I / Dot(pointToShade - m_Pos, pointToShade - m_Pos)
	//--------------------
}
void LPoint::SetPosition(const Elite::FPoint3& newPos)
{
	m_Pos = newPos;
}

void LPoint::Move(const Elite::FVector3& translation)
{
	m_Pos += translation;
}

void LPoint::Update(float elapsedSec)
{
	//Z_RotateAroundPoint(1.f, elapsedSec);
}

//void LPoint::Z_RotateAroundPoint(float radius, float elapsedSec)
//{
//	Elite::FMatrix4 rot = Elite::MakeRotationZ(elapsedSec / 2);
//	Elite::FMatrix4 trans = Elite::MakeTranslation(-Elite::FVector3{ m_Pos});
//	Elite::FVector3 originToPoint{ m_Pos };
//
//	Elite::FPoint3 newPoint;
//	// Translate to world origin + offset
//	// newPoint += originToPoint;
//	newPoint = Elite::FPoint3{ radius, 0.f, 0.f };
//
//	// rotate around
//	newPoint = Elite::FPoint3{ rot * Elite::FPoint4{ newPoint } };
//
//	// InvertTranslate back to lightOrigin
//	newPoint += originToPoint;
//
//	m_Pos = newPoint;
//}