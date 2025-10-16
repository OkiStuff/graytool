#ifndef GRAYSCALE_IMAGE_H
#define GRAYSCALE_IMAGE_H

typedef struct image
{
	unsigned char* data;
	int w;
	int h;
	int bpp;
} image;

image image_read(const char* filename);
void image_write(image* data, const char* filename);
void image_free(image* data);

#endif // GRAYSCALE_IMAGE_H
