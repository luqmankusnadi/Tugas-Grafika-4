#ifndef RECT_H
#define RECT_H

#include "point.h"

typedef struct {
	int x;
	int y;
	int w;
	int h;
} Rect;

void CreateSprite(const Image* image, int w, int h);

#endif
