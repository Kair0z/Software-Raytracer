#pragma once
#include "ERGBColor.h"
#include "EMath.h"

struct HitInfo;

class Material
{
public:
	virtual Elite::RGBColor Shade(const HitInfo& hitInfo, const Elite::FVector3& fromLight, const Elite::FVector3& toView) const = 0;
};

