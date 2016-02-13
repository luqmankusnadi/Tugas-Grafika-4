#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "image.h"
#include "point.h"
#include "rect.h"


typedef struct {
	Image* image;
	Rect bounds;
	Point position;
	float rotation;
	float scale;
} GameObject;

void CreateGameObject(GameObject *gameObject, Image* image);

#endif
