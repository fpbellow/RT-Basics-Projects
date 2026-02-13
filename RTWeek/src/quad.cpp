#include "../include/quad.hpp"

Quad::Quad(const vec3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat) : 
	Q(Q), u(u), v(v), mat(mat)
{
	auto n = cross(u, v);
	normal = unit_vector(n);
	D = dot(normal, Q);
	w = n / dot(n, n);

	set_bounding_box();
}

void Quad::set_bounding_box()
{
	//compute bounding box of all four vertices
	
	auto bbox_diagonal1 = aabb(Q, Q + u + v);
	auto bbox_diagonal2 = aabb(Q + u, Q + v);

	bbox = aabb(bbox_diagonal1, bbox_diagonal2);
}

bool Quad::hit(const ray& r, interval ray_t, geometry_point& rec) const
{
	auto denom = dot(normal, r.direction());

	// no hit if parallel
	if (std::fabs(denom) < 1e-8)
		return false;

	//return false if t is outside ray interval
	auto t = (D - dot(normal, r.origin())) / denom;
	if (!ray_t.contains(t))
		return false;

	//determin if point lies within planar shape
	auto intersection = r.at(t);
	vec3 planar_gpoint_vector = intersection - Q;

	auto alpha = dot(w, cross(planar_gpoint_vector, v));
	auto beta = dot(w, cross(u, planar_gpoint_vector));

	if (!is_interior(alpha, beta, rec))
		return false;

	// ray hits 2d shape, set rest of recorded hit 
	rec.t = t;
	rec.p = intersection;
	rec.mat = mat;
	rec.set_face_normal(r, normal);

	return true;
}

bool Quad::is_interior(float a, float b, geometry_point& rec) const
{
	interval unit_interval = interval(0, 1);

	//check if point is within plane, set UV coords if it is otherwise false

	if (!unit_interval.contains(a) || !unit_interval.contains(b))
		return false;

	rec.u = a;
	rec.v = b;
	return true;
}

shared_ptr<geometry_list> box(const vec3& a, const vec3& b, shared_ptr<material> mat)
{
	//return six sided box with opposite vertices, a & b
	auto sides = make_shared < geometry_list>();

	auto min = vec3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
	auto max = vec3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

	auto dx = vec3(max.x() - min.x(), 0, 0);
	auto dy = vec3(0, max.y() -min.y(), 0);
	auto dz = vec3(0, 0, max.z() - min.z());


	sides->add(make_shared<Quad>(vec3(min.x(), min.y(), max.z()),  dx,  dy, mat));
	sides->add(make_shared<Quad>(vec3(max.x(), min.y(), max.z()), -dz,  dy, mat));
	sides->add(make_shared<Quad>(vec3(max.x(), min.y(), min.z()), -dx,  dy, mat));
	sides->add(make_shared<Quad>(vec3(min.x(), min.y(), min.z()),  dz,  dy, mat));
	sides->add(make_shared<Quad>(vec3(min.x(), max.y(), max.z()),  dx, -dz, mat));
	sides->add(make_shared<Quad>(vec3(min.x(), min.y(), min.z()),  dx,  dz, mat));

	return sides;
}