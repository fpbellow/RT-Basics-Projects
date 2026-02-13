#include "../include/constant_medium.hpp"

constant_medium::constant_medium(shared_ptr<geometry> boundary, float density, shared_ptr<texture> tex) :
	boundary(boundary), neg_inv_density(-1.0f / density), phase_func(make_shared<isotropic>(tex)) { }

constant_medium::constant_medium(shared_ptr<geometry> boundary, float density, const vec3& albedo) :
	boundary(boundary), neg_inv_density(-1.0f/ density), phase_func(make_shared<isotropic>(albedo)) { }


bool constant_medium::hit(const ray& r, interval ray_t, geometry_point& rec) const
{
	geometry_point gp1, gp2;

	if (!boundary->hit(r, interval::universe, gp1))
		return false;

	if (!boundary->hit(r, interval(gp1.t + 0.0001f, infinity), gp2))
		return false;

	if (gp1.t < ray_t.min)
		gp1.t = ray_t.min;

	if (gp2.t > ray_t.max)
		gp2.t = ray_t.max;

	if (gp1.t >= gp2.t)
		return false;

	if (gp1.t < 0)
		gp1.t = 0;


	auto ray_length = r.direction().length();
	auto distance_inside_boundary = (gp2.t - gp1.t) * ray_length;
	auto hit_distance = neg_inv_density * std::log(random_float());

	if (hit_distance > distance_inside_boundary)
		return false;


	rec.t = gp1.t + hit_distance / ray_length;
	rec.p = r.at(rec.t);

	rec.normal = vec3(1, 0, 0); 
	rec.front_face = true;
	rec.mat = phase_func;

	return true;
}