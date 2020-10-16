#include "pch.h"
#include "Ray.h"

Ray::Ray(const Elite::FPoint3& origin, const FVector3& dir, float min, float max, Raytype type)
	: m_Direction{ dir }
	, m_Max{max}
	, m_Min{min}
	, m_Origin{origin}
	, m_Type{type}
{
	Normalize(m_Direction);
}

Ray::Ray(float originX, float originY, float originZ, const FVector3& dir, float min, float max, Raytype type)
	: Ray(Elite::FPoint3{originX, originY, originZ}, dir, min, max, type)
{

}