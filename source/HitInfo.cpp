#include "pch.h"
#include "HitInfo.h"

using namespace Elite;

HitInfo::HitInfo()
	: m_pMaterial{nullptr}
	, m_HitPoint{0, 0, 0}
	, m_Normal{0, 0, 0}
	, m_T{0}
{

}

HitInfo::HitInfo(Material* pMaterial, const FVector3& normal, const FPoint3& hitPoint, float t, float value)
	: m_pMaterial{pMaterial}
	, m_HitPoint{hitPoint}
	, m_Value{ value }
	, m_Normal{normal}
	, m_T{ t }
{

}
