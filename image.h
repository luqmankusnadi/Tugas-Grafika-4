#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "color32.h"
#include "point.h"

typedef struct {
	Color32** pixels;
	int w;
	int h;
	Point pivot;
} Image;

void AllocImage(Image *image, int width, int height);
void FreeImage(Image *image);

#endif
