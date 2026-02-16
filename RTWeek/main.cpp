#include "include/definitions.h"

#include "include/bound_vol_hier.hpp"
#include "include/Camera.hpp"
#include "include/geometry.hpp"
#include "include/geometry_list.h"
#include "include/material.hpp"
#include "include/Sphere.hpp"
#include "include/quad.hpp"
#include "include/constant_medium.hpp"

#include <filesystem>


void bouncing_spheres()
{
	geometry_list world;

	auto checker = make_shared<checker_texture>(0.32f, vec3(.2f, .3f, .1f), vec3(.9f, .9f, .9f));
	world.add(make_shared<Sphere>(vec3(0, -1000.f, 0), 1000.f, make_shared<lambertian>(checker)));

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
					auto center2 = center + vec3(0.0f, random_float(0, 0.5f), 0.0f);
					world.add(make_shared<Sphere>(center, center2, .2f, sphere_material));
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

	world = geometry_list(make_shared<bvh_node>(world));

	Camera cam;

	cam.aspect_ratio = 16.f / 9.f;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.background = vec3(0.7f, 0.8f, 1);

	cam.vfov = 20;
	cam.lookfrom = vec3(13.f, 2.f, 13.f);
	cam.lookat = vec3(0, 0, 0);
	cam.viewUp = vec3(0, 1, 0);

	cam.defocus_angle = .6f;
	cam.focus_dist = 10.f;

	cam.render(world);
}


void checkered_spheres()
{
	geometry_list world;

	auto checker = make_shared<checker_texture>(0.32f, vec3(.2f, .3f, .1f), vec3(.9f, .9f, .9f));

	world.add(make_shared<Sphere>(vec3(.0f, -10.0, 0), 10.0f, make_shared<lambertian>(checker)));
	world.add(make_shared<Sphere>(vec3(.0f, 10.0, 0), 10.0f, make_shared<lambertian>(checker)));

	Camera cam;

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.background = vec3(0.7f, 0.8f, 1);

	cam.vfov = 20;
	cam.lookfrom = vec3(13.0f, 2.0f, 3.0f);
	cam.lookat = vec3(0, 0, 0);
	cam.viewUp = vec3(0, 1, 0);

	cam.defocus_angle = 0;
	
	cam.render(world);
}


void earth()
{
	auto earth_texture = make_shared<image_texture>("assets/textures/earthmap.jpg");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	
	auto globe = make_shared<Sphere>(vec3(0, 0, 0), 2.0f, earth_surface);

	Camera cam;
	
	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.background = vec3(0.7f, 0.8f, 1);

	cam.vfov = 20;
	cam.lookfrom = vec3(0, 0, 12);
	cam.lookat = vec3(0, 0, 0);
	cam.viewUp = vec3(0, 1, 0);

	cam.defocus_angle = 0;
	cam.render(geometry_list(globe));
}


void perlin_spheres()
{
	geometry_list world;
	
	auto perlin_tex = make_shared<noise_texture>(4.0f);

	world.add(make_shared<Sphere>(vec3(0, -1000.0f, 0), 1000.0f, make_shared<lambertian>(perlin_tex)));
	world.add(make_shared<Sphere>(vec3(0, 2.0f, 0), 2.0f, make_shared<lambertian>(perlin_tex)));

	Camera cam;

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.background = vec3(0.7f, 0.8f, 1);

	cam.vfov = 20;
	cam.lookfrom = vec3(13.0f, 2.0f, 3.0f);
	cam.lookat = vec3(0, 0, 0);
	cam.viewUp = vec3(0, 1, 0); 

	cam.defocus_angle = 0;
	
	cam.render(world);
}


void quads()
{
	geometry_list world;

	//materials
	auto left_red    = make_shared<lambertian>(vec3(1.0f, 0.2f, 0.2f));
	auto back_green  = make_shared<lambertian>(vec3(0.2f, 1.0f, 0.2f));
	auto right_blue  = make_shared<lambertian>(vec3(0.2f, 0.2f, 1.0f));
	auto high_orange = make_shared<lambertian>(vec3(1.0f, 0.5f, 0.0f));
	auto low_teal    = make_shared<lambertian>(vec3(0.2f, 0.8f, 0.8f));

	//quads
	world.add(make_shared<Quad>(vec3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
	world.add(make_shared<Quad>(vec3(-2, -2, 0), vec3(4, 0,  0), vec3(0, 4, 0), back_green));
	world.add(make_shared<Quad>(vec3( 3, -2, 1), vec3(0, 0,  4), vec3(0, 4, 0), right_blue));
	world.add(make_shared<Quad>(vec3(-2,  3, 1), vec3(4, 0,  0), vec3(0, 0, 4), high_orange));
	world.add(make_shared<Quad>(vec3(-2, -3, 5), vec3(4, 0,  0), vec3(0, 0,-4), low_teal));

	Camera cam;

	cam.aspect_ratio = 1.0f;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.background = vec3(0.7f, 0.8f, 1);

	cam.vfov = 80;
	cam.lookfrom = vec3(0, 0, 9);
	cam.lookat = vec3(0, 0, 0);
	cam.viewUp = vec3(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}


void simple_light()
{
	geometry_list world;

	auto pertext = make_shared<noise_texture>(4.0f);
	world.add(make_shared<Sphere>(vec3(0, -1000, 0), 1000.0f, make_shared<lambertian>(pertext)));
	world.add(make_shared<Sphere>(vec3(0, 2, 0), 2.0f, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(vec3(4, 4, 4));
	world.add(make_shared<Quad>(vec3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));

	Camera cam;

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.background = vec3(0, 0, 0);

	cam.vfov = 20;
	cam.lookfrom = vec3(26, 3, 6);
	cam.lookat = vec3(0, 2, 0);
	cam.viewUp = vec3(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}


void cornell_box()
{
	geometry_list world;

	auto red   = make_shared<lambertian>(vec3(.65f, .05f, .05f));
	auto white = make_shared<lambertian>(vec3(.73f, .73f, .73f));
	auto green = make_shared<lambertian>(vec3(.12f, .45f, .15f));
	auto light = make_shared<diffuse_light>(vec3(15, 15, 15));

	world.add(make_shared<Quad>(vec3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
	world.add(make_shared<Quad>(vec3(  0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
	world.add(make_shared<Quad>(vec3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0,-105), light));
	world.add(make_shared<Quad>(vec3(  0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
	world.add(make_shared<Quad>(vec3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
	world.add(make_shared<Quad>(vec3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

	shared_ptr<geometry> box1 = box(vec3(0, 0, 0), vec3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15.0f);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	world.add(box1);

	shared_ptr<geometry> box2 = box(vec3(0, 0, 0), vec3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18.0f);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	world.add(box2);


	Camera cam;

	cam.aspect_ratio = 1.0f;
	cam.image_width = 600;
	cam.samples_per_pixel = 200;
	cam.max_depth = 50;
	cam.background = vec3(0, 0, 0);

	cam.vfov = 40;
	cam.lookfrom = vec3(278, 278, -800);
	cam.lookat = vec3(278, 278, 0);
	cam.viewUp = vec3(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}


void cornell_smoke()
{
	geometry_list world;

	auto red = make_shared<lambertian>(vec3(.65f, .05f, .05f));
	auto white = make_shared<lambertian>(vec3(.73f, .73f, .73f));
	auto green = make_shared<lambertian>(vec3(.12f, .45f, .15f));
	auto light = make_shared<diffuse_light>(vec3(7, 7, 7));

	world.add(make_shared<Quad>(vec3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
	world.add(make_shared<Quad>(vec3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
	world.add(make_shared<Quad>(vec3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
	world.add(make_shared<Quad>(vec3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
	world.add(make_shared<Quad>(vec3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
	world.add(make_shared<Quad>(vec3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

	shared_ptr<geometry> box1 = box(vec3(0, 0, 0), vec3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15.0f);
	box1 = make_shared<translate>(box1, vec3(265,0,295));

	shared_ptr<geometry> box2 = box(vec3(0, 0, 0), vec3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18.0f);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));

	world.add(make_shared<constant_medium>(box1, 0.01f, vec3(0, 0, 0)));
	world.add(make_shared<constant_medium>(box2, 0.01f, vec3(1, 1, 1)));

	Camera cam;

	cam.aspect_ratio = 1.0f;
	cam.image_width = 600;
	cam.samples_per_pixel = 200;
	cam.max_depth = 50;
	cam.background = vec3(0, 0, 0);

	cam.vfov = 40;
	cam.lookfrom = vec3(278, 278, -800);
	cam.lookat = vec3(278, 278, 0);
	cam.viewUp = vec3(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}


void week_after_scene(int image_width, int samples_per_pixel, int max_depth)
{
	geometry_list boxes1;
	auto ground = make_shared<lambertian>(vec3(.48f, .83f, .53f));

	int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++)
	{
		for (int j = 0; j < boxes_per_side; j++)
		{
			auto w = 100.0f;
			auto x0 = -1000.0f + i * w;
			auto z0 = -1000.0f + j * w;
			auto y0 = 0.0f;
			auto x1 = x0 + w;
			auto y1 = random_float(1, 101);
			auto z1 = z0 + w;

			boxes1.add(box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground));
		}
	}

	geometry_list world;

	world.add(make_shared<bvh_node>(boxes1));

	auto light = make_shared<diffuse_light>(vec3(7, 7, 7));
	world.add(make_shared<Quad>(vec3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light));

	auto center1 = vec3(400, 400, 200);
	auto center2 = center1 + vec3(30, 0, 0);

	auto sphere_material = make_shared<lambertian>(vec3(.7f, .3f, .1f));
	world.add(make_shared<Sphere>(center1, center2, 50.0f, sphere_material));

	world.add(make_shared<Sphere>(vec3(260, 150, 45), 50.0f, make_shared<dielectric>(1.5f)));
	world.add(make_shared<Sphere>(vec3(0, 150, 145), 50.0f, make_shared<metal>(vec3(.8f, .8f, .9f), 1.0f)));

	auto boundary = make_shared<Sphere>(vec3(360, 150, 145), 70.0f, make_shared<dielectric>(1.5f));
	world.add(boundary);
	world.add(make_shared<constant_medium>(boundary, .2f, vec3(.2f, .4f, .9f)));

	boundary = make_shared<Sphere>(vec3(0, 0, 0), 5000.0f, make_shared<dielectric>(1.5f));
	world.add(make_shared<constant_medium>(boundary, .0001f, vec3(1, 1, 1)));

	auto emat = make_shared<lambertian>(make_shared<image_texture>("assets/textures/earthmap.jpg"));
	world.add(make_shared<Sphere>(vec3(400, 200, 400), 100.0f, emat));
	auto pertext = make_shared<noise_texture>(.2f);
	world.add(make_shared<Sphere>(vec3(220, 280, 300), 80.0f, make_shared<lambertian>(pertext)));

	geometry_list boxes2;
	auto white = make_shared<lambertian>(vec3(.73f, .73f, .73f));
	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxes2.add(make_shared<Sphere>(vec3::random(0, 165), 10.0f, white));
	}

	world.add(make_shared<translate>(make_shared<rotate_y>(make_shared<bvh_node>(boxes2), 15.0f), vec3(-100, 270, 395)));

	Camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = image_width;
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;
	cam.background = vec3(0, 0, 0);

	cam.vfov = 40;
	cam.lookfrom = vec3(478, 278, -600);
	cam.lookat = vec3(278, 278, 0);
	cam.viewUp = vec3(0, 1, 0); 

	cam.defocus_angle = 0;

	cam.render(world);
}


int main()
{
	switch (8)
	{
		case 1: bouncing_spheres(); break;
		case 2: checkered_spheres(); break;
		case 3: earth(); break;
		case 4: perlin_spheres(); break;
		case 5: quads(); break;
		case 6: simple_light(); break;
		case 7: cornell_box(); break;
		case 8: cornell_smoke(); break;
		case 9: week_after_scene(800, 1000, 40); break;
	}
}