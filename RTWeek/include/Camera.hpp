#pragma once

#include <execution>
#include <vector>

#include "geometry.hpp"
#include "interval.hpp"
#include "color.hpp"


class Camera
{
public:
	float aspect_ratio = 1.0f;    // ratio of image width over height
	int image_width = 100;        // rendered image width pixel count
	int samples_per_pixel = 10;   // count of random samples per pixel
	int max_depth = 10;			  // number of ray bounces in scene
	vec3 background = {};

	float vfov = 74.0f;				  // vertical field of view
	vec3 lookfrom = vec3(0, 0, 0);	  // point camera looks from
	vec3 lookat = vec3(0, 0, -1.0f);  // point camera looks at
	vec3 viewUp = vec3(0, 1.0f, 0);   // local up direction for camera

	float defocus_angle = 0;	// variation angle of rays per pixel
	float focus_dist = 10.0;	// distance from cam lookfrom point to plane of focus
	

	void render(const geometry& world);

private:
	int image_height; // rendered image height pixel count
	vec3 center;				// camera center
	vec3 pixel00_location;		// location of (0,0)
	vec3 pixel_delta_u;			// offset to pixel right
	vec3 pixel_delta_v;			// offset to pixel below
	float pixel_samples_scale;	// color scale factor for a sum of pixel samples

	vec3 u, v, w;	// camera frame basis vectors

	vec3 defocus_disk_u;	// defocus disk horizontal radius
	vec3 defocus_disk_v;	// defocus disk vertical radius

	void Initialize();
	
	ray get_ray(int i, int j) const;
	vec3 ray_color(const ray& r, int depth, const geometry& world) const;
	vec3 defocus_disk_sample() const;
};