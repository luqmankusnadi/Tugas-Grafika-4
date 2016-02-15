#include "gameobject.h"

GameObject* CreateGameObject(Image* image){
	GameObject *gameObject = malloc(sizeof(GameObject));
	gameObject->image = image;
	gameObject->position = (Point){0,0};
	gameObject->velocity = (Pointf){0.0f,0.0f};
	gameObject->acceleration = (Pointf){0.0f,0.0f};
	gameObject->rotation = 0.0f;
	gameObject->angularVelocity = 0.0f;
	gameObject->scale = 1.0f;
	gameObject->scalingRate = 0.0f;
	gameObject->bounds = (Rect){-image->pivot.x, -image->pivot.y, image->w, image->h};
	gameObject->parent = NULL;
	gameObject->type = 0;
	VectorInit(&gameObject->childs);
	return gameObject;
}

void UpdateGameObject(GameObject *gameObject){
	gameObject->velocity.x += gameObject->acceleration.x;
	gameObject->velocity.y += gameObject->acceleration.y;
	gameObject->position.x += gameObject->velocity.x;
	gameObject->position.y += gameObject->velocity.y;
	gameObject->rotation += gameObject->angularVelocity;
	SetScale(gameObject, gameObject->scale + gameObject->scalingRate);
}


void SetScale(GameObject* gameObject, float scale){
	float factor = scale / gameObject->scale;
	gameObject->scale = scale;
	gameObject->bounds.x = (int)(-(float)(gameObject->image->pivot.x) * scale);
	gameObject->bounds.y = (int)(-(float)(gameObject->image->pivot.y) * scale);
	gameObject->bounds.w = (int)((float)(gameObject->image->w) * scale);
	gameObject->bounds.h = (int)((float)(gameObject->image->h) * scale);
}

void SetParent(GameObject* child, GameObject* parent){
	child->parent = parent;
	VectorAdd(&parent->childs, child);
}

void DetachAllChilds(GameObject* gameObject){
	int i;
	for(i = 0; i < VectorTotal(&gameObject->childs); i++){
		GameObject *child = VectorGet(&gameObject->childs, i);
		DetachParent(child);
		//child->parent = NULL;
		//VectorDelete(&gameObject->childs, i);
		//i--;
	}
}

void DetachParent(GameObject* gameObject){
	if(gameObject->parent != NULL){
		int i;
		for(i = 0; i< VectorTotal(&gameObject->parent->childs); i++){
			if(gameObject == VectorGet(&gameObject->parent->childs, i)){
				VectorDelete(&gameObject->parent->childs, i);
			}
		}
		float PI = 3.14159265;
		float rad = gameObject->parent->rotation * PI / 180.0f;
		Point localPosition = gameObject->position;
		gameObject->position.x = gameObject->parent->position.x + (int)((float)localPosition.x * cos(rad) - (float)localPosition.y * sin(rad)); 
		gameObject->position.y = gameObject->parent->position.y + (int)((float)localPosition.x * sin(rad) + (float)localPosition.y * cos(rad)); 
		gameObject->scale = gameObject->parent->scale * gameObject->scale;
		gameObject->rotation = fmod(gameObject->parent->rotation + gameObject->rotation, 360);
		gameObject->parent = NULL;
	} 
}

Rect GetGlobalBounds(const GameObject *gameObject){
	Rect bounds;
	bounds.x = gameObject->bounds.x + gameObject->position.x;
	bounds.y = gameObject->bounds.y + gameObject->position.y;
	bounds.w = gameObject->bounds.w;
	bounds.h = gameObject->bounds.h;
	return bounds;
}

Point GetGlobalPosition(const GameObject* gameObject){
	if(gameObject->parent != NULL){
		Point position;
		position.x = gameObject->parent->position.x + gameObject->position.x;
		position.y = gameObject->parent->position.y + gameObject->position.y;
		return position;
	} else {
		return gameObject->position;
	}
}
