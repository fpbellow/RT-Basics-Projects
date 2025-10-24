#ifndef DEFINITIONS_H
#define DEFINITIONS_H


#include <cstdlib>
#include <iostream>


#include "interval.hpp"
#include "ray.h"
#include "common_utils.h"
#include "vec3.h"

//C++ Std Usings
using std::make_shared;
using std::shared_ptr;

//Constants
const float pi = 3.1415926f;


struct gp_data
{
	vec3 p;
	vec3 normal;
	bool front_face;
};

#endif
