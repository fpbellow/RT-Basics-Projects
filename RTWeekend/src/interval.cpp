#include "../include/interval.hpp"

interval::interval() : min(+infinity), max(-infinity) {}

interval::interval(float min, float max) : min(min), max(max) {}

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

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);