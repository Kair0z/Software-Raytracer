#pragma once
#include "ERGBColor.h"
#include "EMath.h"
#include "HitInfo.h"

class Light
{
public:
	Light(const Elite::RGBColor& color, float intensity, bool isOn = true);
	virtual Elite::FVector3 GetDirection(const HitInfo& hitInfo) const = 0;
	virtual Elite::RGBColor BiradianceValue(const HitInfo& hitInfo) const = 0;
	virtual Elite::FPoint3 GetPos() const = 0;
	virtual void Update(float elapsedSec) = 0;

	void Toggle();
	bool IsOn() const;

protected:
	Elite::RGBColor m_Color;
	float m_Intensity;
	bool m_IsOn;
};

