#include "pch.h"
#include "M_LambertCookTorrance.h"
#include "HitInfo.h"
#include "BRDF.h"
#include <iostream>

M_LambertCookTorrance::M_LambertCookTorrance(const Elite::RGBColor& albedo, float roughness, bool isMetal)
	: m_Albedo{albedo}
	, m_IsMetal{isMetal}
	, m_Roughness{roughness}
{
	if (roughness < 0.01f)
	{
		m_Roughness = 0.1f;
	}

	if (roughness > 1.f)
	{
		m_Roughness = 1.f;
	}
}

Elite::RGBColor M_LambertCookTorrance::Shade(const HitInfo& hitInfo, const Elite::FVector3& fromLight, const Elite::FVector3& toView) const
{
	Elite::FVector3 halfVector{ toView + fromLight };

	RGBColor diffuse{ RGBColor{1.f, 1.f, 1.f} - BRDF::FF::Schlick(halfVector, toView, m_Albedo, m_IsMetal) };

	if (m_IsMetal) diffuse = RGBColor{ 0.f, 0.f, 0.f };

	RGBColor specular{ RGBColor{1.f, 1.f, 1.f} - diffuse };

	RGBColor result = BRDF::Lambert(diffuse.r, m_Albedo) * diffuse
		+ BRDF::CookTorrance(m_Albedo, toView, fromLight, hitInfo.m_Normal, m_Roughness, m_IsMetal) * specular;

	return result;
}
