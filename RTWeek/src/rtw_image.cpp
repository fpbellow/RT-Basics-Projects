#include "../include/rtw_image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "../include/stb_image.h"

rtw_image::rtw_image(const char* image_filename)
{
	
	if (load(image_filename)) return;

	std::cerr << "RTW Image Error: Could not load image file " << image_filename << ".\n";
}

rtw_image::~rtw_image()
{
	delete[] bdata;
	STBI_FREE(fdata);
}

bool rtw_image::load(const char* image_filename)
{
	auto n = bytes_per_pixel;
	fdata = stbi_loadf(image_filename, &image_width, &image_height, &n, bytes_per_pixel);
	if (fdata == nullptr) return false;

	bytes_per_scanline = image_width * bytes_per_pixel;
	convert_to_bytes();
	std::cerr << "Loaded " << image_width << "x" << image_height << " from " << image_filename << "\n";
	return true;
}

int rtw_image::width() const
{
	return (fdata == nullptr) ? 0 : image_width;
}

int rtw_image::height() const
{
	return (fdata == nullptr) ? 0 : image_height;
}

const unsigned char* rtw_image::pixel_data(int x, int y) const
{
	static unsigned char magenta[] = { 255, 0, 255 };
	if (bdata == nullptr) return magenta;

	x = clamp(x, 0, image_width);
	y = clamp(y, 0, image_height);

	return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
}

int rtw_image::clamp(int x, int low, int high)
{
	if (x < low) return low;
	if (x < high) return x;
	return high - 1;
}

unsigned char rtw_image::float_to_byte(float value)
{
	if (value <= 0)
		return 0;
	if (1 <= value)
		return 255;

	return static_cast<unsigned char>(256.0 * value);
}

void rtw_image::convert_to_bytes()
{
	int total_bytes = image_width * image_height * bytes_per_pixel;
	bdata = new unsigned char[total_bytes];

	auto *bptr = bdata;
	auto* fptr = fdata;

	for (auto i = 0; i < total_bytes; i++, fptr++, bptr++)
		*bptr = float_to_byte(*fptr);
}