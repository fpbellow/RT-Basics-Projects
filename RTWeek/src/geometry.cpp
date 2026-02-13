#include "../include/geometry.hpp"

void geometry_point::set_face_normal(const ray& r, const vec3& outward_normal)
{
	front_face = dot(r.direction(), outward_normal) < 0;
	normal = front_face ? outward_normal : -outward_normal;
}


translate::translate(shared_ptr<geometry> object, const vec3& offset) : object(object), offset(offset)
{
	bbox = object->bounding_box() + offset;
}

bool translate::hit(const ray& r, interval ray_t, geometry_point& g_point) const
{
	//move ray backwards by the offset
	ray offset_r(r.origin() - offset, r.direction(), r.time());

	//determine if intersection exists along offset ray and where
	if (!object->hit(offset_r, ray_t, g_point))
		return false;

	//move intersection point forwards by offset
	g_point.p += offset;

	return true;
}


rotate_y::rotate_y(shared_ptr<geometry> object, float angle) : object(object)
{
	auto radians = degrees_to_radians(angle);
	sin_theta = std::sin(radians);
	cos_theta = std::cos(radians);

	bbox = object->bounding_box();

	vec3 min(infinity, infinity, infinity);
	vec3 max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
				auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
				auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

				auto x_new = cos_theta * x + sin_theta * z;
				auto z_new = -sin_theta * x + cos_theta * z;

				vec3 tempVec(x_new, y, z_new);

				for (int c = 0; c < 3; c++)
				{
					min[c] = std::fmin(min[c], tempVec[c]);
					max[c] = std::fmax(max[c], tempVec[c]);
				}
			}
		}
	}

	bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, interval ray_t, geometry_point& g_point) const
{
	//transform ray form world to object space

	auto origin = vec3
	(
		(cos_theta * r.origin().x()) - (sin_theta * r.origin().z()),
		r.origin().y(),
		(sin_theta * r.origin().x()) + (cos_theta * r.origin().z())
	);


	auto direction = vec3
	(
		(cos_theta * r.direction().x()) - (sin_theta * r.direction().z()),
		r.direction().y(),
		(sin_theta * r.direction().x()) + (cos_theta * r.direction().z())
	);

	ray rotated_r(origin, direction, r.time());

	//determine if intersection exists in object space and where
	if (!object->hit(rotated_r, ray_t, g_point))
		return false;

	//transform the intersection back to world space

	g_point.p = vec3
	(
		(cos_theta * g_point.p.x()) + (sin_theta * g_point.p.z()),
		g_point.p.y(),
		(-sin_theta * g_point.p.x()) + (cos_theta * g_point.p.z())
	);


	g_point.normal = vec3
	(
		(cos_theta * g_point.normal.x()) + (sin_theta * g_point.normal.z()),
		g_point.normal.y(),
		(-sin_theta * g_point.normal.x()) + (cos_theta * g_point.normal.z())
	);

	return true;
}