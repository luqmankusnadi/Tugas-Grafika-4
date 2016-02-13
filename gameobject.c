#include "gameobject.h"

void CreateGameObject(GameObject *gameObject, Image* image){
	gameObject->image = image;
	gameObject->position.x = 0;
	gameObject->position.y = 0;
	gameObject->rotation = 0.0f;
	gameObject->scale = 1.0f;
	gameObject->bounds.x = -image->pivot.x;
	gameObject->bounds.y = -image->pivot.y;
	gameObject->bounds.w = image->w;
	gameObject->bounds.h = image->h;
}
