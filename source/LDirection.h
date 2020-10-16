#pragma once
#include "Light.h"
class LDirection : public Light
{
public:
	LDirection(const Elite::FVector3& direction, const Elite::RGBColor& color, float intensity, bool isOn = true);

	Elite::FVector3 GetDirection(const HitInfo& hitInfo) const override final;
	Elite::RGBColor BiradianceValue(const HitInfo& hitInfo) const override final;
	Elite::FPoint3 GetPos() const override final; // Why exist? --> So that we don't have to static-cast in running through Lights
	void Update(float elapsedSec) override final;

private:
	Elite::FVector3 m_Direction;
};

