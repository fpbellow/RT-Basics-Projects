#include "../include/material.hpp"

bool material::scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const
{
	return false;
}

vec3 material::emitted(float u, float v, const vec3& p) const
{
	return vec3(0, 0, 0);
}


lambertian::lambertian(const vec3& albedo) : tex(make_shared<solid_color>(albedo)) {}

lambertian::lambertian(shared_ptr<texture> tex) : tex(tex) {}

bool lambertian::scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const
{
	auto scatter_direction = geo_data.normal + random_unit_vector();

	if (scatter_direction.near_zero())
		scatter_direction = geo_data.normal;

	scattered = ray(geo_data.p, scatter_direction, r_in.time());
	attenuation = tex->value(geo_data.u, geo_data.v, geo_data.p);
	return true;
}


metal::metal(const vec3& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

bool metal::scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const
{
	vec3 reflected = reflect(r_in.direction(), geo_data.normal);
	reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
	scattered = ray(geo_data.p, reflected, r_in.time());
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

	scattered = ray(geo_data.p, direction, r_in.time());
	return true;
}

float dielectric::reflectance(float cosine, float refraction_index)
{
	auto r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
}



diffuse_light::diffuse_light(shared_ptr<texture> tex) : tex(tex) {}

diffuse_light::diffuse_light(const vec3& emit) : tex(make_shared<solid_color>(emit)) {}

vec3 diffuse_light::emitted(float u, float v, const vec3& p) const
{
	return tex->value(u, v, p);
}


isotropic::isotropic(const vec3& albedo) : tex(make_shared<solid_color>(albedo)) {}

isotropic::isotropic(shared_ptr<texture> tex) : tex(tex) {}

bool isotropic::scatter(const ray& r_in, const gp_data geo_data, vec3& attenuation, ray& scattered) const
{
	scattered = ray(geo_data.p, random_unit_vector(), r_in.time());
	attenuation = tex->value(geo_data.u, geo_data.v, geo_data.p);
	return true;
}
