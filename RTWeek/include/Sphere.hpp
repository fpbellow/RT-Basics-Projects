#pragma once

#include "definitions.h"
#include "geometry.hpp"

class Sphere : public geometry
{
public:
	//stationary
	Sphere(const vec3& static_center, float radius, shared_ptr<material> mat);

	//moving
	Sphere(const vec3& center1, const vec3& center2, float radius, shared_ptr<material> mat);

	bool hit(const ray& r, interval ray_t, geometry_point& g_point) const override;

	aabb bounding_box() const override { return bbox; }

private:
	ray center;
	float radius;
	shared_ptr<material> mat;
	aabb bbox;

	static void get_sphere_uv(const vec3& p, float& u, float& v);
};