#include "../include/Sphere.hpp"

Sphere::Sphere(const vec3& static_center, float radius, shared_ptr<material> mat) : center(static_center, vec3(0.0f, 0.0f, 0.0f)), radius(std::fmax(.0f, radius)), mat(mat)
{
	auto rvec = vec3(radius, radius, radius);
	bbox = aabb(static_center - rvec, static_center + rvec);
}

Sphere::Sphere(const vec3& center1, const vec3& center2, float radius, shared_ptr<material> mat) : center(center1, center2 - center1), radius(std::fmax(.0f, radius)), mat(mat)
{
	auto rvec = vec3(radius, radius, radius);
	aabb box1(center.at(0) - rvec, center.at(0) + rvec);
	aabb box2(center.at(1) - rvec, center.at(1) + rvec);
	bbox = aabb(box1, box2);
}

bool Sphere::hit(const ray& r, interval ray_t, geometry_point& g_point) const 
{
	vec3 current_center = center.at(r.time());
	vec3 oc = current_center - r.origin();
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
	vec3 outward_normal = (g_point.p - current_center) / radius;
	g_point.set_face_normal(r, outward_normal);
	get_sphere_uv(outward_normal, g_point.u, g_point.v);
	g_point.mat = mat;

	return true;
}

void Sphere::get_sphere_uv(const vec3& p, float& u, float& v)
{
	auto theta = std::acos(-p.y());
	auto phi = std::atan2(-p.z(), p.x()) + pi;

	u = phi / (2 * pi);
	v = theta / pi;
}