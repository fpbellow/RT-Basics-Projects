#pragma once

#include "definitions.h"
#include "material.hpp"

class geometry_point
{
public:
	vec3 p;
	vec3 normal;
	shared_ptr<material> mat;
	float t;
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class geometry
{
public:
	virtual ~geometry() = default;

	virtual bool hit(const ray& r, interval ray_t, geometry_point& g_point) const = 0;
};