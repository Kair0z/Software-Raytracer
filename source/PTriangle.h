#pragma once
#include "Primitive.h"
#include <vector>

enum class Cullmode
{
	Backface = 1, Frontface = -1, NoCull = 0
	// The face mentioned doesn't show!
};

class PTriangle : public Primitive
{
public:
	// Absolute point locations
	PTriangle(const std::vector<FPoint3>& points, Material* pMaterial, bool isVisible = true, Cullmode cullMode = Cullmode::NoCull); // Points counterclockwise order!
	PTriangle(const FPoint3& p0, const FPoint3& p1, const FPoint3& p2, Material* pMaterial, bool isVisible = true, Cullmode cullMode = Cullmode::NoCull); // Points counterclockwise order!

	// relative point locations
	PTriangle(const FPoint3& origin, std::vector<FPoint3> rel_Points, Material* pMaterial, bool isVisible = true, Cullmode cullMode = Cullmode::NoCull);
	PTriangle(const FPoint3& origin, const FPoint3& rel_P0, const FPoint3& rel_P1, const FPoint3& rel_P2, Material* pMaterial, bool isVisible = true, Cullmode cullMode = Cullmode::NoCull);

	bool Hit(const Ray& ray, HitInfo& hitInfo) const override;
	void Rotate(float x, float y, float z) override final;
	void RotateAroundPoint(float x, float y, float z, const FPoint3& pivotPoint);

	static bool StaticHit(const Ray& ray, HitInfo& hitInfo, const FVector3& normal, const std::vector<FPoint3>& points, Cullmode cullmode);

	Cullmode GetCullMode() const;

private:
	std::vector<FPoint3> m_Abs_Points;
	FVector3 m_Normal;
	Cullmode m_Cullmode;
};

