#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"


#include "../include/Camera.hpp"

vec3 sample_square() 
{
	//return vec3 to random point in [-.5f, -.5f] to [+.5f, +.5f] unit square
	return vec3(random_float() - 0.5f, random_float() - 0.5f, 0);
}



//void Camera::render(const geometry& world)
//{
//	Initialize();
//
//	std::unique_ptr<unsigned char[]> image_data(new unsigned char[image_width * image_height * 3]);
//
//	for (int j = 0; j < image_height; j++)
//	{
//
//		for (int i = 0; i < image_width; i++)
//		{
//			vec3 pixel_color(0, 0, 0);
//			for (int sample = 0; sample < samples_per_pixel; sample++)
//			{
//				ray r = get_ray(i, j);
//				pixel_color += ray_color(r, max_depth, world);
//			}
//
//			int pixel_index = (j * image_width + i) * 3;
//			vec3 result_color = calculate_color(pixel_samples_scale * pixel_color);
//			//vec3 result_color(0,0,0);
//
//			image_data[pixel_index + 0] = static_cast<unsigned char>(result_color.r());
//			image_data[pixel_index + 1] = static_cast<unsigned char>(result_color.g());
//			image_data[pixel_index + 2] = static_cast<unsigned char>(result_color.b());
//		}
//	}
//
//
//	stbi_write_png("result.png", image_width, image_height, 3, image_data.get(), image_width * 3);
//}

void Camera::render(const geometry& world)
{
	Initialize();

	std::unique_ptr<unsigned char[]> image_data(new unsigned char[image_width * image_height * 3]);

	std::vector<int> pixels(image_width * image_height);
	std::iota(pixels.begin(), pixels.end(), 0);

	std::for_each(std::execution::par, pixels.begin(), pixels.end(),
		[&](int pixel_index)
		{
			int j = pixel_index / image_width;
			int i = pixel_index % image_width;

			vec3 pixel_color(0, 0, 0);
			for (int sample = 0; sample < samples_per_pixel; sample++)
			{
				ray r = get_ray(i, j); 
				pixel_color += ray_color(r, max_depth, world);
			}

			vec3 result_color = calculate_color(pixel_samples_scale * pixel_color);
			//vec3 result_color(0,0,0);

			image_data[pixel_index * 3 + 0] = static_cast<unsigned char>(result_color.r());
			image_data[pixel_index * 3 + 1] = static_cast<unsigned char>(result_color.g());
			image_data[pixel_index * 3 + 2] = static_cast<unsigned char>(result_color.b());
		});


	stbi_write_png("result.png", image_width, image_height, 3, image_data.get(), image_width * 3);
}

void Camera::Initialize()
{
	image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	pixel_samples_scale = 1.0f / samples_per_pixel;

	center = lookfrom;

	//determine viewport dimensions

	auto theta = degrees_to_radians(vfov);
	auto h = std::tan(theta * 0.5f);
	auto viewport_height = 2 * h * focus_dist;
	auto viewport_width = viewport_height * (static_cast<float>(image_width) / image_height);

	//calculate u,v,w unit vectors for camera 
	w = unit_vector(lookfrom - lookat); //unit vec with opposite view direction
	u = unit_vector(cross(viewUp, w)); //unit vec with camera right direction
	v = cross(w, u); //unit vec with camera up direction


	//calculate vectors across the edges
	auto viewport_u = viewport_width * u;
	auto viewport_v = viewport_height * -v;

	//calculate delta vectors between pixels
	pixel_delta_u = viewport_u / static_cast<float>(image_width);
	pixel_delta_v = viewport_v / static_cast<float>(image_height);

	auto viewport_upper_left = center - (focus_dist * w) - (viewport_u * .5f) - (viewport_v * .5f);
	pixel00_location = viewport_upper_left + .5f * (pixel_delta_u + pixel_delta_v);

	//calculate camera defocus disk basis vectors
	auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle * 0.5f));
	defocus_disk_u = u * defocus_radius;
	defocus_disk_v = v * defocus_radius;
}

ray Camera::get_ray(int i, int j) const
{
	//construct camera ray from defocus disc at randomly sampled point near i,j

	auto offset = sample_square();
	auto pixel_sample = pixel00_location +
		((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

	auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
	auto ray_direction = pixel_sample - ray_origin;

	return ray(ray_origin, ray_direction);
}



vec3 Camera::ray_color(const ray& r, int depth, const geometry& world) const
{
	if (depth <= 0)
		return vec3(0, 0, 0);

	geometry_point g_point;

	if (world.hit(r, interval(0.001f, infinity), g_point))
	{
		ray scattered;
		vec3 attenuation;
		
		gp_data geo_data = { g_point.p, g_point.normal, g_point.front_face };


		if (g_point.mat->scatter(r, geo_data, attenuation, scattered))
		{
			return attenuation * ray_color(scattered, depth - 1, world);
		}
		return vec3(0, 0, 0);
		
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - a) * vec3(1.0f, 1.0f, 1.0f) + a * vec3(0.5f, 0.7f, 1.0f);
}


vec3 Camera::defocus_disk_sample() const
{
	//returns a random point in the defocus disk
	auto p = random_in_unit_disk();
	return center + (p.x() * defocus_disk_u) + (p.y() * defocus_disk_v);
}