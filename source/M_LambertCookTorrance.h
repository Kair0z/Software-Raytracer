#pragma once
#include "Material.h"
class M_LambertCookTorrance : public Material
{
public:
	M_LambertCookTorrance(const Elite::RGBColor& albedo, float roughness, bool isMetal);
	Elite::RGBColor Shade(const HitInfo& hitInfo, const Elite::FVector3& fromLight, const Elite::FVector3& toView) const override final;

private:
	Elite::RGBColor m_Albedo;
	float m_Roughness;
	bool m_IsMetal;
};

