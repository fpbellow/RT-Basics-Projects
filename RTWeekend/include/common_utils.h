#pragma once

#include <cmath>
#include <limits>
#include <memory>

//Utility Functions
inline float degrees_to_radians(float degrees)
{
	return degrees * 3.1415926f / 180.0f;
}

inline float random_float()
{
	//retruns a random real within [0,1)
	return std::rand() / (RAND_MAX + 1.0f);
}

inline float random_float(float min, float max)
{
	//return random real in [min, max)
	return min + (max - min) * random_float();
}

