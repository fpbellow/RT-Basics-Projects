#include "../include/texture.hpp"

vec3 checker_texture::value(float u, float v, const vec3& p) const
{
	auto xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
	auto yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
	auto zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

	bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

	return isEven ? even->value(u, v, p) : odd->value(u, v, p);
}


vec3 image_texture::value(float u, float v, const vec3& p) const
{
	//if no texture data, return solid cyan
	if (image.height() <= 0) return vec3(0, 1, 1);

	//clamp input texture coordinates to [0,1] x [1,0]
	u = interval(0, 1).clamp(u);
	v = 1.0f - interval(0, 1).clamp(v);

	auto i = static_cast<int>(u * image.width());
	auto j = static_cast<int>(v * image.height());

	auto pixel = image.pixel_data(i, j);

	auto color_scale = 1.0f / 255.0f;
	return vec3(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}


vec3 noise_texture::value(float u, float v, const vec3& p) const
{
	return vec3(.5f, .5f, .5f) * (1 + std::sin(scale * p.z() + 10.0f * noise.turb(p, 7)));
}