#pragma once

#include "definitions.h"
#include "texture.hpp"

class material
{
public:
	virtual ~material() = default;

	virtual vec3 emitted(float v, float u, const vec3& p) const;

	virtual bool scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const;
};


class lambertian : public material
{
public:
	lambertian(const vec3& albedo);
	lambertian(shared_ptr<texture> tex);

	bool scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const override;

private:
	vec3 albedo;
	shared_ptr<texture> tex;
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


class diffuse_light : public material
{
public:
	diffuse_light(shared_ptr<texture> tex);
	diffuse_light(const vec3& emit);

	vec3 emitted(float u, float v, const vec3& p) const override;

private:
	shared_ptr<texture> tex;
};


class isotropic : public material
{
public:
	isotropic(const vec3& albedo);
	isotropic(shared_ptr<texture> tex);

	bool scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const override;

private:
	shared_ptr<texture> tex;
};