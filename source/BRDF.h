#pragma once
#include "ERGBColor.h"

using namespace Elite;

namespace BRDF
{
	RGBColor Lambert(float diffuseReflectance, const RGBColor& diffuseColor);
	RGBColor Phong(float specularReflectance, float phongExp, const FVector3& fromLight, const FVector3& toView, const FVector3& hitNormal);
	RGBColor CookTorrance(const RGBColor& albedo, const FVector3& toView, const FVector3& fromLight, const FVector3& hitNormal, float roughness, bool isMetal, bool directLighting = true);

	// Normal-Distribution-functions
	namespace NDF
	{
		float TrowbridgeReitz(const FVector3& hitNormal, const FVector3& halfVector, float roughness_2);
	}

	// Fresnel-functions
	namespace FF
	{
		RGBColor Schlick(const FVector3& halfVector, const FVector3& toView, const RGBColor& albedo, bool isMetal);
	}

	// Geometry-functions
	namespace GF
	{
		float Schlick(const FVector3& hitNormal, const FVector3& toView, float k);
	}

}
