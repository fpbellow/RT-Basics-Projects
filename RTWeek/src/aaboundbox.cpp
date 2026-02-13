#include "../include/aaboundbox.hpp"

aabb::aabb(const vec3& a, const vec3& b)
{

	x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
	y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
	z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);

	pad_to_minimums();
}

aabb::aabb(const aabb& box0, const aabb& box1)
{
	x = interval(box0.x, box1.x);
	y = interval(box0.y, box1.y);
	z = interval(box0.z, box1.z);
}

const interval& aabb::axis_interval(int n) const
{
	if (n == 1) return y;
	if (n == 2) return z;
	return x;
}

bool aabb::hit(const ray& r, interval ray_t) const
{
	const vec3& ray_orig = r.origin();
	const vec3& ray_dir = r.direction();

	for (int axis = 0; axis < 3; axis++)
	{
		const interval& ax = axis_interval(axis);
		const float adInv = 1.0f / ray_dir[axis];

		auto t0 = (ax.min - ray_orig[axis]) * adInv;
		auto t1 = (ax.max - ray_orig[axis]) * adInv;

		if (t0 < t1)
		{
			if (t0 > ray_t.min) ray_t.min = t0;
			if (t1 < ray_t.max) ray_t.max = t1;
		} 
		else
		{
			if (t1 > ray_t.min) ray_t.min = t1;
			if (t0 < ray_t.max) ray_t.max = t0;
		}

		if (ray_t.max <= ray_t.min)
			return false;
	}

	return true;
}

int aabb::longest_axis() const
{
	//Return the index of longes axis of bounding box
	if (x.size() > y.size())
		return x.size() > z.size() ? 0 : 2;
	else
		return y.size() > z.size() ? 1 : 2;
}

void aabb::pad_to_minimums()
{
	//adjust box so no side is narrower than some delta, padding if necessary

	float delta = 0.0001f;
	
	if (x.size() < delta) x = x.expand(delta);
	if (y.size() < delta) y = y.expand(delta);
	if (z.size() < delta) z = z.expand(delta);
}


const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

aabb operator+ (const aabb& bbox, const vec3& offset)
{
	return aabb(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
}

aabb operator- (const vec3& offset, const aabb& bbox)
{
	return bbox + offset;
}