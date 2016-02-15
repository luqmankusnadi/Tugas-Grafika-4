#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <math.h>
#include "image.h"
#include "point.h"
#include "rect.h"
#include "vector.h"

typedef struct GameObject GameObject;

struct GameObject{
	Image* image;
	Rect bounds;
	Point position;
	Pointf velocity;
	Pointf acceleration;
	float rotation;
	float angularVelocity;
	float scale;
	float scalingRate;
	GameObject *parent;
	Vector childs;
	int type;
};

GameObject* CreateGameObject(Image* image);
void UpdateGameObject(GameObject *gameObject);
void SetScale(GameObject* gameObject, float scale);
void SetParent(GameObject* child, GameObject* parent);
void DetachAllChilds(GameObject* gameObject);
void DetachParent(GameObject* gameObject);
Rect GetGlobalBounds(const GameObject *gameObject);
Point GetGlobalPosition(const GameObject* gameObject);


#endif
