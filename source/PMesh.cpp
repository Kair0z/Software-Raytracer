#include "pch.h"
#include "PMesh.h"
#include <fstream>
#include <iostream>
#include "PTriangle.h"
#include "HitInfo.h"
#include "Ray.h"
#include "Material.h"
#include <regex>
#include "Utils.h"
#include <string>
#include <sstream>

enum class Linetype
{
	Comment, Vertex, Face, Unknown
};

PMesh::PMesh(const std::string& filename, Material* pMaterial, const FPoint3& origin, bool isVisible, Cullmode cullmode, float scale)
	: Primitive(origin, pMaterial, isVisible)
	, m_Cullmode{cullmode}
	, m_Scale{ scale }
{
	std::vector<std::string> comments = ParseFile(filename);

	/*for (std::string comment : comments)
	{
		std::cout << comment << "\n";
	}*/

	if (!Elite::AreEqual(scale, 1.f))
	{
		Scale(scale);
	}

	PrecomputeFaceNormals();
	CalculateBoundingBallRadius();
}

PMesh::~PMesh()
{
	delete m_pMaterial;
	m_pMaterial = nullptr;
}

void PMesh::PrecomputeFaceNormals()
{
	for (Face& face : m_IdxBuffer)
	{
		// 0 -> 1 & 0 -> 2
		FVector3 a = m_VertexBuffer[face.m_Val2].m_Point - m_VertexBuffer[face.m_Val0].m_Point;
		FVector3 b = m_VertexBuffer[face.m_Val1].m_Point - m_VertexBuffer[face.m_Val0].m_Point;

		face.m_Normal = Elite::Cross(b, a);
		Elite::Normalize(face.m_Normal);

		// PP cross example tricks you!
	}
}

void PMesh::CaptureVertex(const std::string& pointString)
{
	std::stringstream ss{ pointString };

	Vertex temp{ unsigned int(m_VertexBuffer.size()), FPoint3{} };

	float tempFloat{};

	for (int i{}; i < 3; ++i)
	{
		ss >> tempFloat;
		temp.m_Point[i] = tempFloat;
	}

	m_VertexBuffer.push_back(temp);

}

void PMesh::CaptureFace(const std::string& faceString)
{
	std::stringstream ss{ faceString };

	Face temp{};
	int tempInt{};

	for (int i{}; i < 3; ++i)
	{
		ss >> tempInt;
		temp[i] = tempInt - 1;
	}

	m_IdxBuffer.push_back(temp);
}

std::vector<std::string> PMesh::ParseFile(const std::string& filename)
{
	std::string filepath{ "Resources/" + filename + ".obj" };
	std::ifstream input(filepath, std::ios::in);

	std::vector<std::string> returnComments;

	if (!input)
	{
		std::cout << "Error loading file! " << filepath << " \n";
		return returnComments;
	}

	std::string line{};
	Linetype lineType{};

	std::string signature{};
	std::vector<std::string> capturedGroups;

	while (!input.eof())
	{
		std::getline(input, line);

		std::string regString{ "^\\s*(\\S)\\s+(.*)" };
		std::regex captureSignRegex{ regString };
		capturedGroups = Utils::GetCaptureGroups(line, captureSignRegex);

		// if we captured something:
		if (capturedGroups.size() > 0)
		{
			signature = capturedGroups[1];

			// Set up linetype
			if (signature == "#") lineType = Linetype::Comment;
			else if (signature == "v") lineType = Linetype::Vertex;
			else if (signature == "f") lineType = Linetype::Face;
			else lineType = Linetype::Unknown;

			//std::cout << capturedGroups[2] << "\n"; Actual content of the line (#: string of text), (v: 3 numbers), ...

			switch (lineType)
			{
			case Linetype::Vertex:
				CaptureVertex(capturedGroups[2]);
				break;

			case Linetype::Comment:
				returnComments.push_back(capturedGroups[2]);
				break;

			case Linetype::Face:
				CaptureFace(capturedGroups[2]);
				break;

			case Linetype::Unknown:
				//std::cout << "Unknown linetype found in file! \n";
				//std::cout << line << "\n";
				break;
			}

		}
	}

	return returnComments;
}

bool PMesh::Hit(const Ray& ray, HitInfo& hitInfo) const
{
	// Return if no vertices exist in the mesh
	if (m_VertexBuffer.size() == 0) return false;

	// if the not shadow-ray falls outside the ball, return false. 
	if (ray.m_Type != Raytype::Shadow && !FallsInBoundingBall(ray, hitInfo)) return false;

	// Order of hit setup
	float closestT{ ray.m_Max };
	HitInfo closestHitInfo{};

	// don't return untill all faces have been checked! --> Order of hit also applies to a Mesh
	bool tempHit{ false };

	// Check faces
	for (Face face : m_IdxBuffer)
	{
		// Vertexbuffer keeps relative positions towards origin --> We need absolute points for hit calculation!
		std::vector<FPoint3> absolutePoints{ m_Origin + FVector3{m_VertexBuffer[face.m_Val0].m_Point}, m_Origin + FVector3{m_VertexBuffer[face.m_Val1].m_Point}, m_Origin + FVector3{m_VertexBuffer[face.m_Val2].m_Point} };

		// we don't need to actually make objects of Triangles, just need to check with parameters
		if (PTriangle::StaticHit(ray, closestHitInfo, face.m_Normal, absolutePoints, m_Cullmode)) 
		{
			if (closestHitInfo.m_T < closestT)
			{
				closestT = closestHitInfo.m_T;
				hitInfo = closestHitInfo;
				hitInfo.m_pMaterial = m_pMaterial;

				tempHit = true;
			}
			
		}
	}

	return tempHit;
}

void PMesh::Rotate(float x, float y, float z)
{
	FMatrix4 rot = Elite::MakeRotationZYX(x, y, z);

	for (Vertex& vertex : m_VertexBuffer)
	{
		vertex.m_Point = FPoint3{ rot * FPoint4{ vertex.m_Point } };
	}

	// Recalculate the face normals
	PrecomputeFaceNormals();
}

void PMesh::Scale(float newScale)
{
	if (newScale < 0.f || Elite::AreEqual(newScale, 0.f))
	{
		m_IsVisible = false;
		m_Scale = 0.f;
		return;
	}

	m_Scale = newScale;

	m_BoundingBallRadius *= newScale;

	FMatrix4 scale = Elite::MakeScale(newScale, newScale, newScale);

	for (Vertex& vertex : m_VertexBuffer)
	{
		vertex.m_Point = FPoint3{ scale * FPoint4{ vertex.m_Point } };
	}

	PrecomputeFaceNormals();
}

float PMesh::GetScale() const
{
	return m_Scale;
}

void PMesh::IncScale(float increment)
{
	Scale(m_Scale + increment);
}

bool PMesh::FallsInBoundingBall(const Ray& ray, HitInfo& hitInfo) const
{
	return PSphere::StaticHit(m_Origin, m_BoundingBallRadius, ray, hitInfo);
}

void PMesh::CalculateBoundingBallRadius()
{
	float longestScale{};
	float tempScale{};

	for (Vertex& vertex : m_VertexBuffer)
	{
		tempScale = Elite::Magnitude(FVector3{ vertex.m_Point });
	
		if (tempScale > longestScale)
		{
			longestScale = tempScale;
		}
	}

	m_BoundingBallRadius = longestScale;
}
