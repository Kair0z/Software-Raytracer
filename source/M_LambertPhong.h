#pragma once
#include "Material.h"
class M_LambertPhong : public Material
{
public:
	M_LambertPhong(const Elite::RGBColor& baseColor, float specReflectance, float diffReflectance, float phongExp);
	Elite::RGBColor Shade(const HitInfo& hitInfo, const Elite::FVector3& fromLight, const Elite::FVector3& toView) const override final;

private:
	float m_PhongExp;
	float m_SpecularReflectance;
	float m_DiffuseReflectance;
	Elite::RGBColor m_BaseColor;
};

