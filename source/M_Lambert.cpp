#include "pch.h"
#include "M_Lambert.h"
#include "BRDF.h"

M_Lambert::M_Lambert(const Elite::RGBColor& baseColor, float diffReflectance)
	: m_BaseColor{baseColor}
	, m_DiffuseReflectance{diffReflectance}
{

}

Elite::RGBColor M_Lambert::Shade(const HitInfo& hitInfo, const Elite::FVector3& fromLight, const Elite::FVector3& toView) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_BaseColor);
}