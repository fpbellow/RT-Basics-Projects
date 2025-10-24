#include "../include/Sphere.hpp"

Sphere::Sphere(const vec3& center, float radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0.0f, radius)), mat(mat)
{

}

bool Sphere::hit(const ray& r, interval ray_t, geometry_point& g_point) const 
{
	vec3 oc = center - r.origin();
	auto a = r.direction().length_squared();
	auto h = dot(r.direction(), oc);
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = h * h - a * c;
	if (discriminant < 0)
		return false;

	auto sqrtd = std::sqrt(discriminant);

	auto root = (h - sqrtd) / a;
	if (!ray_t.surrounds(root))
	{
		root = (h + sqrtd) / a;
		if (!ray_t.surrounds(root))
			return false;
	}

	g_point.t = root;
	g_point.p = r.at(g_point.t);
	vec3 outward_normal = (g_point.p - center) / radius;
	g_point.set_face_normal(r, outward_normal);
	g_point.mat = mat;

	return true;
}