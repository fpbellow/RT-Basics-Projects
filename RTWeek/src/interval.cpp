#include "../include/interval.hpp"

interval::interval() : min(+infinity), max(-infinity) {}

interval::interval(float min, float max) : min(min), max(max) {}

interval::interval(const interval& a, const interval& b)
{
	min = a.min <= b.min ? a.min : b.min;
	max = a.max >= b.max ? a.max : b.max;
}

float interval::size() const
{
	return max - min;
}

bool interval::contains(float x) const
{
	return min <= x && x <= max;
}

bool interval::surrounds(float x) const
{
	return min < x && x < max;
}

float interval::clamp(float x) const
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

interval interval::expand(float delta) const
{
	auto padding = delta * .5f;
	return interval(min - padding, max + padding);
}

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

interval operator+ (const interval& ival, float displacement)
{
	return interval(ival.min + displacement, ival.max + displacement);
}

interval operator- (float displacement, const interval& ival)
{
	return ival + displacement;
}