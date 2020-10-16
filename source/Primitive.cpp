#include "pch.h"
#include "Primitive.h"

Primitive::Primitive(const FPoint3& origin, Material* pMaterial, bool isVisible)
	: m_Origin{origin}
	, m_pMaterial{pMaterial}
	, m_IsVisible{isVisible}
{

}

const Material* Primitive::GetMaterial() const
{
	return m_pMaterial;
}

bool Primitive::IsVisible() const
{
	return m_IsVisible;
}