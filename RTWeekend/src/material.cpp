#include "../include/material.hpp"

bool material::scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const
{
	return false;
}


lambertian::lambertian(const vec3& albedo) : albedo(albedo) {}

bool lambertian::scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const
{
	auto scatter_direction = geo_data.normal + random_unit_vector();

	if (scatter_direction.near_zero())
		scatter_direction = geo_data.normal;

	scattered = ray(geo_data.p, scatter_direction);
	attenuation = albedo;
	return true;
}


metal::metal(const vec3& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

bool metal::scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const
{
	vec3 reflected = reflect(r_in.direction(), geo_data.normal);
	reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
	scattered = ray(geo_data.p, reflected);
	attenuation = albedo;
	return (dot(scattered.direction(), geo_data.normal) > 0);
}


dielectric::dielectric(float refraction_index) : refraction_index(refraction_index) {}

bool dielectric::scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const
{
	attenuation = vec3(1.0f, 1.0f, 1.0f);
	float ri = geo_data.front_face ? (1.0f / refraction_index) : refraction_index;

	vec3 unit_direction = unit_vector(r_in.direction());

	float cos_theta = std::fmin(dot(-unit_direction, geo_data.normal), 1.0f);
	float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

	bool cannot_refract = ri * sin_theta > 1.0f;
	vec3 direction;

	if (cannot_refract)
		direction = reflect(unit_direction, geo_data.normal);
	else
		direction = refract(unit_direction, geo_data.normal, ri);

	scattered = ray(geo_data.p, direction);
	return true;
}

float dielectric::reflectance(float cosine, float refraction_index)
{
	auto r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
}