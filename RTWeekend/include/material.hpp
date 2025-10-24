#pragma once

#include "definitions.h"

class material
{
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const;
};


class lambertian : public material
{
public:
	lambertian(const vec3& albedo);

	bool scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const override;

private:
	vec3 albedo;
};


class metal : public material
{
public:
	metal(const vec3& albedo, float fuzz);

	bool scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const override;

private:
	vec3 albedo;
	float fuzz;
};


class dielectric : public material
{
public:
	dielectric(float refraction_index);

	bool scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const override;

	static float reflectance(float cosine, float refraction_index);

private:
	float refraction_index;
};