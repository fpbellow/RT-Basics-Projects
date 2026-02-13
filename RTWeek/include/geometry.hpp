#pragma once

#include "definitions.h"
#include "material.hpp"
#include "aaboundbox.hpp"

class geometry_point
{
public:
	vec3 p;
	vec3 normal;
	shared_ptr<material> mat;
	float t, u, v;
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal);
};


class geometry
{
public:
	virtual ~geometry() = default;

	virtual bool hit(const ray& r, interval ray_t, geometry_point& g_point) const = 0;

	virtual aabb bounding_box() const = 0;
};


class translate : public geometry
{
public:
	translate(shared_ptr<geometry> object, const vec3& offset);

	bool hit(const ray& r, interval ray_t, geometry_point& g_point) const override;

	aabb bounding_box() const override { return bbox; }

private:
	shared_ptr<geometry> object;
	vec3 offset;
	aabb bbox;
};


class rotate_y : public geometry
{
public:

	rotate_y(shared_ptr<geometry> object, float angle);

	bool hit(const ray& r, interval ray_t, geometry_point& g_point) const override;

	aabb bounding_box() const override { return bbox; }

private:
	shared_ptr<geometry> object;
	float sin_theta = 0;
	float cos_theta = 0;
	aabb bbox;
};