#pragma once

#include "definitions.h"
#include "perlin.hpp"
#include "rtw_image.hpp"

class texture
{
public:

	virtual ~texture() = default;

	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};


class solid_color : public texture
{
public:
	solid_color(const vec3& albedo) : albedo(albedo) {}

	solid_color(float red, float green, float blue) : solid_color(vec3(red, green, blue)) {}

	vec3 value(float u, float v, const vec3& p) const override { return albedo; }

private:
	vec3 albedo;
};


class checker_texture : public texture
{
public:
	checker_texture(float scale, shared_ptr<texture> even, shared_ptr<texture> odd) :
		inv_scale(1.0f / scale), even(even), odd(odd) { }

	checker_texture(float scale, const vec3& c1, const vec3& c2) :
		checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) { }

	vec3 value(float u, float v, const vec3& p) const override;

private:
	float inv_scale;
	shared_ptr<texture> even;
	shared_ptr<texture> odd;
};


class image_texture : public texture
{
public:
	image_texture(const char* filename) : image(filename) {}

	vec3 value(float u, float v, const vec3& p) const override;

private:
	rtw_image image;
};


class noise_texture : public texture
{
public:
	noise_texture(float scale) : scale(scale) {}

	vec3 value(float u, float v, const vec3& p) const override;

private:
	perlin noise;
	float scale;
};