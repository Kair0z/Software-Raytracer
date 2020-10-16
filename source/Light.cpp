#include "pch.h"
#include "Light.h"

Light::Light(const Elite::RGBColor& color, float intensity, bool isOn)
	: m_Color{color}
	, m_Intensity{intensity}
	, m_IsOn{isOn}
{
	m_Color.Clamp();
}

void Light::Toggle()
{
	m_IsOn = !m_IsOn;
}

bool Light::IsOn() const
{
	return m_IsOn;
}