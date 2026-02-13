#pragma once

#include "geometry.hpp"
#include "geometry_list.h"

class Quad : public geometry
{
public:
	Quad(const vec3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat);

	virtual void set_bounding_box();

	aabb bounding_box() const override { return bbox; }

	bool hit(const ray& r, interval ray_t, geometry_point& rec) const override;

	virtual bool is_interior(float a, float b, geometry_point& rec) const;

private:
	vec3 Q, u, v, w;
	shared_ptr<material> mat;
	aabb bbox;
	vec3 normal;
	float D;
};

shared_ptr<geometry_list> box(const vec3& a, const vec3& b, shared_ptr<material> mat);