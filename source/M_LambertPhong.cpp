#include "pch.h"
#include "M_LambertPhong.h"
#include "BRDF.h"
#include "HitInfo.h"

M_LambertPhong::M_LambertPhong(const Elite::RGBColor& baseColor, float specReflectance, float diffReflectance, float phongExp)
	: m_BaseColor{baseColor}
	, m_PhongExp{phongExp}
	, m_SpecularReflectance{specReflectance}
	, m_DiffuseReflectance{diffReflectance}
{

}

Elite::RGBColor M_LambertPhong::Shade(const HitInfo& hitInfo, const Elite::FVector3& fromLight, const Elite::FVector3& toView) const 
{
	return BRDF::Lambert(m_DiffuseReflectance, m_BaseColor)
		+ BRDF::Phong(m_SpecularReflectance, m_PhongExp, fromLight, toView, hitInfo.m_Normal);
}