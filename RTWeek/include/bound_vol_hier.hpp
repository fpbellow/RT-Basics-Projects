#pragma once

#include "aaboundbox.hpp"
#include "geometry.hpp"
#include "geometry_list.h"

#include <algorithm>

class bvh_node : public geometry
{
public:
	bvh_node(geometry_list list) : bvh_node(list.objects, 0, list.objects.size())
	{ 
	}

	bvh_node(std::vector<shared_ptr<geometry>>& objects, size_t start, size_t end);

	bool hit(const ray& r, interval ray_t, geometry_point& rec) const override;

	aabb bounding_box() const override { return bbox; }


private:
	shared_ptr<geometry> left;
	shared_ptr<geometry> right;
	aabb bbox;

	static bool box_compare(const shared_ptr<geometry> a, const shared_ptr<geometry> b, int axis_index);

	static bool box_x_compare(const shared_ptr<geometry> a, const shared_ptr<geometry> b);
	static bool box_y_compare(const shared_ptr<geometry> a, const shared_ptr<geometry> b);
	static bool box_z_compare(const shared_ptr<geometry> a, const shared_ptr<geometry> b);
};