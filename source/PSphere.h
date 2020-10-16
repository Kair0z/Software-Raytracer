#pragma once
#include "Primitive.h"

class PSphere : public Primitive
{
public:
	PSphere(const Elite::FPoint3& origin, float radius, Material* pMaterial, bool isVisible = true);
	bool Hit(const Ray& ray, HitInfo& hitInfo) const override;

	static bool StaticHit(const FPoint3& origin, float radius, const Ray& ray, HitInfo& hitInfo);

	void Rotate(float x, float y, float z) override final;


private:
	float m_Radius;

};

