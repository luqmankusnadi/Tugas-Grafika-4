#ifndef RECT_H
#define RECT_H

#include "point.h"

typedef struct {
	int x;
	int y;
	int w;
	int h;
} Rect;

int RectContains(Rect rect, Point point);
int RectOverlaps(Rect rect1, Rect rect2);

#endif
