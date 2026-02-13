#pragma once

#include "geometry.hpp"
#include "material.hpp"
#include "texture.hpp"

class constant_medium : public geometry
{
public:
	constant_medium(shared_ptr<geometry> boundary, float density, shared_ptr<texture> tex);

	constant_medium(shared_ptr<geometry> boundary, float density, const vec3& albedo);

	bool hit(const ray& r, interval ray_t, geometry_point& rec) const override;

	aabb bounding_box() const override { return boundary->bounding_box(); }

private:
	shared_ptr<geometry> boundary;
	float neg_inv_density;
	shared_ptr<material> phase_func;
};