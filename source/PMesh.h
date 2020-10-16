#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "EMath.h"
#include "PTriangle.h"
#include "PSphere.h"

struct Ray;
struct HitInfo;
class Material;

using namespace Elite;

struct Vertex
{
	unsigned int m_Idx;
	FPoint3 m_Point;
};

struct Face
{
	Face() = default;
	Face(int v0, int v1, int v2, const FVector3& normal) : m_Val0{ v0 }, m_Val1{ v1 }, m_Val2{ v2 }, m_Normal{normal}{}

	int m_Val0;
	int m_Val1;
	int m_Val2;

	FVector3 m_Normal;

	int& operator[](int idx)
	{
		switch (idx)
		{
		case 1:
			return m_Val1;
			break;

		case 2:
			return m_Val2;
			break;

		case 0:
			return m_Val0;
			break;
		}

		return m_Val2;
	}
};

class PMesh : public Primitive
{
public:
	PMesh(const std::string& filename, Material* pMaterial, const FPoint3& origin = FPoint3{ 0.f, 0.f, 0.f }, bool isVisible = true, Cullmode cullmode = Cullmode::NoCull, float scale = 1.f); // Load from .obj file
	~PMesh();

	bool Hit(const Ray& ray, HitInfo& hitInfo) const override final;
	void Rotate(float x, float y, float z) override final;
	void Scale(float newScale);
	void IncScale(float increment); // Not recommended, original scale isn't kept track of, so it grows exponentially
	float GetScale() const;

private:
	std::vector<std::string> ParseFile(const std::string& filename);
	void CaptureFace(const std::string& faceString);
	void CaptureVertex(const std::string& vertexString);

	void PrecomputeFaceNormals();
	bool FallsInBoundingBall(const Ray& ray, HitInfo& hitInfo) const;
	void CalculateBoundingBallRadius();

	std::vector<Vertex> m_VertexBuffer;
	std::vector<Face> m_IdxBuffer;

	float m_BoundingBallRadius;

	Cullmode m_Cullmode;
	float m_Scale;
};

