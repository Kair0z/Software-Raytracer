#pragma once
#include "EMath.h"

using namespace Elite;

struct Quaternion
{
	explicit Quaternion(float x, float y, float z, float w);

	float Length() const;

	void Normalize();
	Quaternion GetConjugate() const;

	Quaternion operator*(const Quaternion& rhs) const;

	FVector3 RotateVector(const FVector3& vector, float angle) const;

	float m_X, m_Y, m_Z, m_W;
	// x, y, z = i, j, k 
	// w = scalar
};

