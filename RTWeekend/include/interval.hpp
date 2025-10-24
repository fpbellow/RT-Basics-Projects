#pragma once

#include <limits>

const float infinity = std::numeric_limits<float>::infinity();

class interval
{
public:

	float min, max;

	interval();

	interval(float min, float max);
	
	float size() const;
	bool contains(float x) const;
	bool surrounds(float x) const;
	float clamp(float x) const;

	static const interval empty, universe;
};

