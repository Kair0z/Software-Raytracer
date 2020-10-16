#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Material.h"

struct HitInfo
{
	HitInfo();
	HitInfo(Material* material, const Elite::FVector3& normal, const Elite::FPoint3& hitPoint, float t, float value);

	Elite::FPoint3 m_HitPoint;
	Elite::FVector3 m_Normal;
	Material* m_pMaterial;
	float m_Value;
	float m_T;
};

