#pragma once
#include "Light.h"
#include "HitInfo.h"
#include "EMath.h"

class LPoint : public Light
{
public:
	LPoint(const Elite::FPoint3& pos, const Elite::RGBColor& color, float intensity, bool isOn = true);

	Elite::FVector3 GetDirection(const HitInfo& hitInfo) const override final;
	Elite::RGBColor BiradianceValue(const HitInfo& hitInfo) const override final;
	void Update(float elapsedSec) override final;

	Elite::FPoint3 GetPos() const override final
	{
		return m_Pos;
	}

	void SetPosition(const Elite::FPoint3& newPos);
	void Move(const Elite::FVector3& translation);

private:
	Elite::FPoint3 m_Pos;

	// Unfinished Rotation around a pivot offsetted!
	//void Z_RotateAroundPoint(float radius, float elapsedSec);
};

