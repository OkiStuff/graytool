#include "generator.h"
#include <stdio.h>
#include <string.h>

#define R (index + 0)
#define G (index + 1)
#define B (index + 2)
#define A (index + 3)

#define MAX3(a,b,c) ((a) > (b) ? \
					(a) > (c) ? (a) : (c) : \
					(b) > (c) ? (b) : (c))

#define MIN3(a,b,c) ((a) < (b) ? \
					(a) < (c) ? (a) : (c) :\
					(b) < (c) ? (b) : (c))

static const char method_simple_names[][20] =
{
	[GENERATOR_METHOD_AVERAGE] = "average",
	[GENERATOR_METHOD_LUMINANCE601] = "luminance601",
	[GENERATOR_METHOD_LUMINANCE709] = "luminance709",
	[GENERATOR_METHOD_LUMINANCE_CUSTOM] = "luminance[%f,%f,%f]",
	[GENERATOR_METHOD_DESATURATION] = "desaturation",
	[GENERATOR_METHOD_VALUE] = "value"
};

void generator_method_to_simple_name(char* out, size_t out_size, generator_method_descriptor* method)
{
	const char* name = method_simple_names[method->method];

	if (method->method == GENERATOR_METHOD_LUMINANCE_CUSTOM)
	{
		snprintf(out, out_size, name, method->custom_weights[0], method->custom_weights[1], method->custom_weights[2]);
	}

	else
	{
		strncpy_s(out, out_size, name, out_size - 1);
	}
}

void generator_generate_grayscale(unsigned char* out, image* image, generator_method_descriptor* method)
{
	for (int y = 0; y < image->h; y++)
	{
		for (int x = 0; x < image->w; x++)
		{
			int index = (y * image->w + x) * image->bpp;

			double red = image->data[R] / 255.0;
			double green = image->data[G] / 255.0;
			double blue = image->data[B] / 255.0;

			double y = 0.0;

			switch (method->method)
			{
			case GENERATOR_METHOD_AVERAGE:
				y = (red + green + blue) / 3;
				break;

			case GENERATOR_METHOD_LUMINANCE601:
				y = (0.299 * red) + (0.587 * green) + (0.114 * blue);
				break;

			case GENERATOR_METHOD_LUMINANCE709:
				y = (0.2126 * red) + (0.7152 * green) + (0.0722 * blue);
				break;

			case GENERATOR_METHOD_LUMINANCE_CUSTOM:
				y = (method->custom_weights[0] * red) + (method->custom_weights[1] * green) + (method->custom_weights[2] * blue);
				break;

			case GENERATOR_METHOD_DESATURATION:
				y = (MAX3(red, green, blue) + MIN3(red, green, blue)) / 2;
				break;

			case GENERATOR_METHOD_VALUE:
				y = MAX3(red, green, blue);
				break;
			}

			int value = y * 255;

			out[R] = value;
			out[G] = value;
			out[B] = value;
		}
	}
}
