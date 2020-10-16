#pragma once
#include "EMathUtilities.h"
#include "EMath.h"

using namespace Elite;

enum class Raytype
{
	View,
	Shadow
};

struct Ray
{
	Ray(const FPoint3& origin, const FVector3& dir, float min = 0.0001f, float max = FLT_MAX, Raytype type = Raytype::View);
	Ray(float originX, float originY, float originZ, const FVector3& dir, float min = 0.0001f, float max = FLT_MAX, Raytype type = Raytype::View);

	FPoint3 m_Origin;
	FVector3 m_Direction;
	float m_Min; // min t "[)"
	float m_Max; // max t "[)"

	Raytype m_Type;
};


