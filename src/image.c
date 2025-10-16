#include "image.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

image image_read(const char* filename)
{
	int w, h, bpp;
	unsigned char* data = stbi_load(filename, &w, &h, &bpp, 4);

	return (image)
	{
		.data = data,
		.w = w,
		.h = h,
		.bpp = 4,//bpp
	};
}

void image_write(image* data, const char* filename)
{
	stbi_write_png(filename, data->w, data->h, data->bpp, data->data, data->w * data->bpp);
}

void image_free(image* data)
{	
	stbi_image_free(data->data);
	
	data->data = NULL;
	data->w = 0;
	data->h = 0;
	data->bpp = 0;
}
