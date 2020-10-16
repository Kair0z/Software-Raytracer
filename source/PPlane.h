#pragma once
#include "Primitive.h"

using namespace Elite;

class PPlane : public Primitive
{
public:
	PPlane(const FPoint3& origin, const FVector3& normal, Material* pMaterial, bool isVisible = true);
	bool Hit(const Ray& ray, HitInfo& hitInfo) const override;

	void Rotate(float x, float y, float z) override final; // Not implemented

private:
	FVector3 m_Normal;
};

