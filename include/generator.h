#ifndef GRAYTOOL_GENERATOR_H
#define GRAYTOOL_GENERATOR_H

#include "image.h"
#include <stdint.h>

typedef enum generator_method
{
	GENERATOR_METHOD_AVERAGE = 0,
	GENERATOR_METHOD_LUMINANCE601,
	GENERATOR_METHOD_LUMINANCE709,
	GENERATOR_METHOD_LUMINANCE_CUSTOM,
	GENERATOR_METHOD_DESATURATION,
	GENERATOR_METHOD_VALUE
} generator_method;

typedef struct generator_method_descriptor
{
	generator_method method;
	double custom_weights[3]; // Only used for Luminance (custom)
} generator_method_descriptor;

void generator_method_to_simple_name(char* out, size_t out_size, generator_method_descriptor* method);
void generator_generate_grayscale(unsigned char* out, image* image, generator_method_descriptor* method);

#endif // GRAYTOOL_GENERATOR
