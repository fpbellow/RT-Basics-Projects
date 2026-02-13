#pragma once

#include "definitions.h"

class aabb
{
public:

	interval x, y, z;

	aabb() {}

	aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) 
	{
		pad_to_minimums();
	}

	aabb(const vec3& a, const vec3& b);

	aabb(const aabb& box0, const aabb& box1);

	const interval& axis_interval(int n) const;

	bool hit(const ray& r, interval ray_t) const;

	int longest_axis() const;

	static const aabb empty, universe;


private:

	void pad_to_minimums();

};

aabb operator+ (const aabb& bbox, const vec3& offset);
aabb operator- (const vec3& offset, const aabb& bbox);