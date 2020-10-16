#include "pch.h"
#include "BRDF.h"

Elite::RGBColor BRDF::Lambert(float diffuseReflectance, const Elite::RGBColor& diffuseColor)
{
	return diffuseColor * diffuseReflectance / float(E_PI);
}

Elite::RGBColor BRDF::Phong(float specularReflectance, float phongExp, const Elite::FVector3& fromLight, const Elite::FVector3& toView, const Elite::FVector3& hitNormal)
{
	// normal must be normalized here!
	Elite::FVector3 reflectVector{ -fromLight + 2 * Elite::Dot(hitNormal, fromLight) * hitNormal };

	Elite::RGBColor result{ specularReflectance, specularReflectance, specularReflectance };
	result *= powf(Elite::Dot(reflectVector, toView), phongExp);

	return result;
}

Elite::RGBColor BRDF::CookTorrance(const RGBColor& albedo, const Elite::FVector3& toView, const Elite::FVector3& fromLight, const Elite::FVector3& hitNormal, float roughness, bool isMetal, bool directLighting)
{
	Elite::FVector3 halfVector{toView + fromLight};
	Elite::Normalize(halfVector);
	// angle between either & halfvector can't be bigger than 90° (PI / 2)

	float remappedRoughness{};

	if (directLighting) remappedRoughness = powf((roughness + 1), 2) / 8.f;
	else remappedRoughness = powf(roughness, 2) / 2.f;

	// 1-line alternative for ifstatement
	remappedRoughness = powf(roughness + (directLighting * 1), 2.f) / 2.f + (directLighting * 6);

	float normalDistribution{NDF::TrowbridgeReitz(hitNormal, halfVector, powf(roughness, 2.f))};
	Elite::RGBColor fresnel{FF::Schlick(halfVector, toView, albedo, isMetal)};
	float geometry{GF::Schlick(hitNormal, toView, remappedRoughness) * GF::Schlick(hitNormal, fromLight, remappedRoughness)};

	Elite::RGBColor result = (fresnel * geometry * normalDistribution) / (4 * (Elite::Dot(toView, hitNormal) * (Elite::Dot(fromLight, hitNormal))));

	return result;
}


#pragma region normal-distribution

float BRDF::NDF::TrowbridgeReitz(const FVector3& hitNormal, const FVector3& halfVector, float roughness_2)
{
	float result{};

	float roughness_2_2{ powf(roughness_2, 2.f) };

	float c1{ powf(Elite::Dot(hitNormal, halfVector), 2) };
	float c2{ roughness_2_2 - 1 };

	result = roughness_2_2 / (float(E_PI) * powf(c1 * c2 + 1, 2.f));

	return result;
}

#pragma endregion

#pragma region fresnel

RGBColor BRDF::FF::Schlick(const FVector3& halfVector, const FVector3& toView, const RGBColor& albedo, bool isMetal)
{
	RGBColor result{};
	RGBColor F0{ albedo };

	if (!isMetal) F0 = RGBColor{ 0.4f, 0.4f, 0.4f };

	RGBColor c1{ RGBColor{ 1.f, 1.f, 1.f } - F0 };
	float c2{ powf(1 - (Elite::Dot(halfVector, toView)), 5.f) };

	result = F0 + c1 * c2;

	return result;
}

#pragma endregion

#pragma region geometry

float BRDF::GF::Schlick(const FVector3& hitNormal, const FVector3& toView, float k)
{
	float result{};

	result = Elite::Dot(hitNormal, toView) / (Elite::Dot(hitNormal, toView) * (1 - k) + k);

	return result;
}

#pragma endregion


