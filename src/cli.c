#include "cli.h"
#include "generator.h"
#include "image.h"
#include "version.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// buffer/target_length should be the amount of characters, not the amount of allocated characters. Equal to strlen(str)
static inline uint8_t starts_with(const char* buffer, size_t buffer_length, const char* target, size_t target_length)
{
	return buffer_length >= target_length && strncmp(buffer, target, target_length) == 0;
}

void cli_version()
{
	printf("graytool " GRAYTOOL_VERSION " - Authored by Frankie A. <https://github.com/okistuff/graytool>\n");
}

void cli_help(const char* executable)
{
	cli_version();
	printf(
	    "Usage: %s [options] <file>\n\n"
	    "Options:\n"
	    "\t-h/--help\t\t\tShow this message\n"
	    "\t-v/--version\t\t\tShow version\n"
	    "\t-o/--output <output file>\tSet output file\n\n"
	    "\t--average\t\t\t\t\tUse Average method\n"
	    "\t--luminance601 (default)\t\t\tUse Luminance (Rec.601) method\n"
	    "\t--luminance709\t\t\t\t\tUse Luminance (Rec.709) method\n"
	    "\t--luminance <R weight>,<G weight>,<B weight>\tUse Luminance method with custom weights\n"
	    "\t--desaturation\t\t\t\t\tUse Desaturation method\n"
	    "\t--value\t\t\t\t\t\tUse Value method\n", executable);
}

cli_option cli_parse_option(const char* option)
{
	char* shifted_option_ptr = (char*)(option) + 1; // We will assume '-' was already checked

	if (strcmp(shifted_option_ptr, "h") == 0 || strcmp(shifted_option_ptr, "-help") == 0)
	{
		return CLI_OPTION_HELP;
	}

	if (strcmp(shifted_option_ptr, "v") == 0 || strcmp(shifted_option_ptr, "-version") == 0)
	{
		return CLI_OPTION_VERSION;
	}

	if (strcmp(shifted_option_ptr, "o") == 0 || strcmp(shifted_option_ptr, "-output") == 0)
	{
		return CLI_OPTION_OUTPUT;
	}

	if (option[1] == '-')
	{
		shifted_option_ptr++;
		
		if (strcmp(shifted_option_ptr, "average") == 0)
		{
			return CLI_OPTION_AVERAGE;
		}

		if (strcmp(shifted_option_ptr, "luminance601") == 0)
		{
			return CLI_OPTION_LUMINANCE601;
		}

		if (strcmp(shifted_option_ptr, "luminance709") == 0)
		{
			return CLI_OPTION_LUMINANCE709;
		}

		if (strcmp(shifted_option_ptr, "luminance") == 0)
		{
			return CLI_OPTION_LUMINANCE_CUSTOM;
		}

		if (strcmp(shifted_option_ptr, "desaturation") == 0)
		{
			return CLI_OPTION_DESATURATION;
		}

		if (strcmp(shifted_option_ptr, "value") == 0)
		{
			return CLI_OPTION_VALUE;
		}
	}

	return CLI_OPTION_NONE;
}

int cli_run(int argc, const char** argv)
{
	if (argc == 1)
	{
		cli_help(argv[0]);
		return 0;
	}

	generator_method_descriptor method = (generator_method_descriptor){GENERATOR_METHOD_LUMINANCE601};
	char weights[INPUT_MAX_LEN];
	
	char output[INPUT_MAX_LEN];
	output[0] = '\0';
	
	int options = 0;

	while (1 + options < argc && argv[1 + options][0] == '-')
	{
		cli_option result = cli_parse_option(argv[1 + options]);
		int ret = 0;

		switch (result)
		{
		case CLI_OPTION_NONE:
			printf("Error: Unknown option %s\n\n", argv[1]);
			ret = 1;
			// Drops down to CLI_OPTION_HELP

		case CLI_OPTION_HELP:			
			cli_help(argv[0]);
			return ret;

		case CLI_OPTION_VERSION:
			cli_version();
			return ret;

		case CLI_OPTION_OUTPUT:
			if (argc < 3 + options)
			{
				printf("Error: Not enough arguments for --output. Expected 2, found 1.\nUsage: %s --output <output_file> ...\n", argv[0]);
				return 1;
			}
			
			strncpy_s(output, INPUT_MAX_LEN, argv[2 + options], INPUT_MAX_LEN - 1);
			
			options++;
			break;

		case CLI_OPTION_LUMINANCE_CUSTOM:
			if (argc < 3 + (options * 2))
			{
				printf("Error: Not enough arguments for --luminance. Expected 2, found 1.\nUsage: %s --luminance <R weight>,<G weight>,<B weight> ...\n", argv[0]);
				return 0;
			}
			
			method.method = GENERATOR_METHOD_LUMINANCE_CUSTOM;
			
			strncpy_s(weights, INPUT_MAX_LEN, argv[2 + options], INPUT_MAX_LEN - 1);

			char* weightsptr = weights;
			char* endptr;

			int i = 0;

			do
			{
				double value = strtod(weightsptr, &endptr);

				if (endptr == weightsptr)
				{
					printf("Error: malformed argument for --luminance.\nUsage: %s --luminace <R weight>,<G weight>,<B weight> ...\n", argv[0]);
					return 0;
				}

				if (*endptr == ',')
				{
					weightsptr = endptr + 1;
				}

				method.custom_weights[i++] = value;
			} while (*endptr != '\0' && i < 3);

			options++;
			break;

		default: // All other method options
			method.method = result - CLI_OPTION_AVERAGE;
			break;
		}

		options++;
	}

	if (argc < options + 2) // should have all the first arg (executable), options, and then another arg
	{
		printf("Error: Not enough arguments. Expected %d, found %d\nUsage: %s [options] <file>", options + 1, argc - 1, argv[0]);
	}

	const int after_options_offset = 1 + options;
	char filename[INPUT_MAX_LEN];

	strncpy_s(filename, INPUT_MAX_LEN, argv[after_options_offset], INPUT_MAX_LEN - 1);

	char method_name_buffer[40];
	generator_method_to_simple_name(method_name_buffer, 40, &method);

	if (output[0] == '\0')
	{		
		snprintf(output, INPUT_MAX_LEN, "grayscale(%s)-%s", method_name_buffer, filename);
	}

	image image = image_read(filename);

	if (image.data == NULL)
	{
		printf("Error: Failed to load \"%s\"\n", filename);
		return 0;
	}

	printf("Generating \"%s\" with %s... ", output, method_name_buffer);

	generator_generate_grayscale(image.data, &image, &method);
	
	image_write(&image, output);
	image_free(&image);

	printf("finished\n");
	
	return 0;
}
