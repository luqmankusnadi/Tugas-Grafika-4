#include "rect.h"

int RectContains(Rect rect, Point point){
	if(point.x >= rect.x && point.x < rect.x + rect.w && point.y >= rect.y && point.y < rect.y + rect.h)
		return 1;
	else
		return 0;
}

int RectOverlaps(Rect rect1, Rect rect2){
	if(rect1.x + rect1.w < rect2.x || rect2.x + rect2.w < rect1.x || rect1.y + rect1.h < rect2.y || rect2.y + rect2.h < rect1.y)
		return 0;
	else
    	return 1;
}
