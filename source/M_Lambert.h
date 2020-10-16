#pragma once
#include "Material.h"
class M_Lambert : public Material
{
public:
	M_Lambert(const Elite::RGBColor& baseColor, float diffReflectance);
	Elite::RGBColor Shade(const HitInfo& hitInfo, const Elite::FVector3& fromLight, const Elite::FVector3& toView) const override final;

private:
	Elite::RGBColor m_BaseColor;
	float m_DiffuseReflectance;
};

