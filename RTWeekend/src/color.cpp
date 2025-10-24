#include "../include/color.hpp"

float linear_to_gamma(float linear_component)
{
	if (linear_component > 0)
		return std::sqrt(linear_component);

	return 0;
}

vec3 calculate_color(vec3 pixel_color)
{
	static const interval intensity(0.0f, 0.999f);
	auto r = pixel_color.r();
	auto g = pixel_color.g();
	auto b = pixel_color.b();

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	return vec3(256 * intensity.clamp(r), 256 * intensity.clamp(g), 256 * intensity.clamp(b));
}