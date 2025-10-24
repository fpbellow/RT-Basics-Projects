#pragma once

#include "definitions.h"
#include "geometry.h"

#include <vector>

using std::make_shared;
using std::shared_ptr;

class geometry_list : public geometry
{
public:
	std::vector<shared_ptr<geometry>> objects;

	geometry_list() {}
	geometry_list(shared_ptr<geometry> object) { add(object); }

	void clear() { objects.clear(); }

	void add(shared_ptr<geometry> object)
	{
		objects.push_back(object);
	}

	bool hit(const ray& r, interval ray_t, geometry_point& g_point) const override
	{
		geometry_point temp_point;
		bool hit_anything = false;

		auto closest_so_far = ray_t.max;

		for (const auto& object : objects)
		{
			if (object->hit(r, interval(ray_t.min,  closest_so_far), temp_point))
			{
				hit_anything = true;
				closest_so_far = temp_point.t;
				g_point = temp_point;
			}
		}

		return hit_anything;
	}
};