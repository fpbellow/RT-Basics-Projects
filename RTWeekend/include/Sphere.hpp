#pragma once

#include "definitions.h"
#include "geometry.h"

class Sphere : public geometry
{
public:
	Sphere(const vec3& center, float radius, shared_ptr<material> mat);

	bool hit(const ray& r, interval ray_t, geometry_point& g_point) const override;

private:
	vec3 center;
	float radius;
	shared_ptr<material> mat;
};