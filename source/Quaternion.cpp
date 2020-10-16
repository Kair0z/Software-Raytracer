#include "pch.h"
#include "Quaternion.h"

Quaternion::Quaternion(float x, float y, float z, float w)
	: m_X{x}
	, m_Y{y}
	, m_Z{z}
	, m_W{w}
{

}

float Quaternion::Length() const
{
	return float(sqrt(m_X * m_X + m_Y * m_Y + m_Z * m_Z + m_W * m_W));
}

void Quaternion::Normalize()
{
	float length{ Length() };

	m_X /= length;
	m_Y /= length;
	m_Z /= length;
	m_W /= length;
}

Quaternion Quaternion::GetConjugate() const
{
	return Quaternion{ -m_X, -m_Y, -m_Z, m_W };
}

Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
	Quaternion result{
		m_W* rhs.m_X + m_X * rhs.m_W + m_Y * rhs.m_Z - m_Z * rhs.m_Y,
		m_W* rhs.m_Y + m_X * rhs.m_Z + m_Y * rhs.m_W + m_Z * rhs.m_X,
		m_W* rhs.m_Z + m_X * rhs.m_Y - m_Y * rhs.m_X + m_Z * rhs.m_W,
		m_W* rhs.m_W - m_X * rhs.m_X - m_Y * rhs.m_Y - m_Z * rhs.m_Z
	};

	return result;
}

FVector3 Quaternion::RotateVector(const FVector3& vector, float angle) const
{
	// Quaternion --> Subject-vector
	Quaternion qVec{
		m_X,
		m_Y,
		m_Z,
		0.f
	};

	// RotationQuaternion around 'vector'
	Quaternion Rot{
		vector.x * float(sin(Elite::ToRadians(angle) / 2.f)),
		vector.y * float(sin(Elite::ToRadians(angle) / 2.f)),
		vector.z * float(sin(Elite::ToRadians(angle) / 2.f)),
		float(cos(Elite::ToRadians(angle) / 2.f))
	};

	Quaternion qResult = Rot * qVec * Rot.GetConjugate();

	return FVector3{ qResult.m_X, qResult.m_Y, qResult.m_Z };
}