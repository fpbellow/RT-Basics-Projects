#include "include/definitions.h"

#include "include/Camera.hpp"
#include "include/geometry.h"
#include "include/geometry_list.h"
#include "include/material.hpp"
#include "include/Sphere.hpp"



int main()
{
	geometry_list world;

	auto ground_material = make_shared<lambertian>(vec3(.5f, .5f, .5f));
	world.add(make_shared<Sphere>(vec3(0, -1000.f, 0), 1000.f, ground_material));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			auto choose_mat = random_float();
			vec3 center(a + .9f * random_float(), .2f, b + .9f * random_float());

			if ((center - vec3(4, .2f, 0)).length() > .9f)
			{
				shared_ptr<material> sphere_material;

				if (choose_mat < .8f)
				{
					//diffuse
					auto albedo = vec3::random() * vec3::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<Sphere>(center, .2f, sphere_material));
				}
				else if (choose_mat < .95f)
				{
					//metal
					auto albedo = vec3::random(.5f, 1.f);
					auto fuzz = random_float(0, .5f);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(center, .2f, sphere_material));
				}
				else
				{
					//glass
					sphere_material = make_shared<dielectric>(1.5f);
					world.add(make_shared<Sphere>(center, .2f, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5f);
	world.add(make_shared<Sphere>(vec3(0, 1.f, 0), 1.f, material1));

	auto material2 = make_shared<lambertian>(vec3(.4f, .2f, .1f));
	world.add(make_shared<Sphere>(vec3(-4.f, 1.f, 0), 1.f, material2));

	auto material3 = make_shared<metal>(vec3(.7f, .6f, .5f), 0.f);
	world.add(make_shared<Sphere>(vec3(4.f, 1.f, 0), 1.f, material3));

	Camera cam;

	cam.aspect_ratio = 16.f / 9.f;
	cam.image_width = 1000;
	cam.samples_per_pixel = 50;
	cam.max_depth = 50;

	cam.vfov = 20;
	cam.lookfrom = vec3(13.f, 2.f, 13.f);
	cam.lookat = vec3(0, 0, 0);
	cam.viewUp = vec3(0, 1, 0);

	cam.defocus_angle = .6f;
	cam.focus_dist = 10.f;

	cam.render(world);
}