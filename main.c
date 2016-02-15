#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "framebuffer.h"
#include "image.h"
#include "gameobject.h"
#include "bitmap.h"
#include "input.h"
#include "vector.h"

//Function untuk missile
int missile_finished_launched(int x,int y,int width_background){	
	return y < 0 || x < 0 || x > width_background;
}

//screen size
long int width, height;

float ToRadians(float degrees){
	float PI = 3.14159265;
	return degrees * PI / 180.0f;
}

int main(){	
	InitFramebuffer(&width, &height);
	// Image spaceshipImage = LoadBitmapAsImage("spaceship.bmp");
	Image parachuteImage = LoadBitmapAsImage("parachute.bmp");
	parachuteImage.pivot.y = 0;
	Image backgroundImage = LoadBitmapAsImage("background.bmp");
	Image rocketImage = LoadBitmapAsImage("rocket.bmp");
	Image planeImage = LoadBitmapAsImage("plane.bmp");
	Image propellerImage = LoadBitmapAsImage("propeller.bmp");
	Image wheelImage = LoadBitmapAsImage("wheel.bmp");
	
	Vector gameObjects;
	VectorInit(&gameObjects);

	GameObject* cannon = CreateGameObject(&rocketImage);
	cannon->position = (Point){width/2, height};
	VectorAdd(&gameObjects, cannon);
	
	set_conio_terminal_mode();
	int i;
	
	int rot = 0;
	int rot_offset = 6;
	int rot_max = 50;
	
	while(1){
		ClearScreen();
	
		rot = ((rot + rot_offset)% rot_max);
		if (rot_max - rot < 3){
			rot_offset *=  -1;
		}
		else if (rot_max - rot > 97){
			rot_offset *=  -1;
		}
		if(rand() % 50 == 0){
			GameObject *plane = CreateGameObject(&planeImage);
			GameObject *propeller = CreateGameObject(&propellerImage);
			plane->position.x = width;
			plane->position.y = 100;
			plane->type = 1;
			SetParent(propeller, plane);
			plane->velocity.x = (-(rand() % 5))-5;
			plane->velocity.y = (rand() % 3);
			plane->scalingRate = 0.01;
			propeller->angularVelocity = 30;
			VectorAdd(&gameObjects, plane);
			VectorAdd(&gameObjects, propeller);
		} 
		if(kbhit()){
			char c = getch();
			if(c == 'a'){
				if(cannon->rotation > - 80)
				cannon->rotation -= 10;
			} 
			else if(c == 'd'){
				if(cannon->rotation <  80)
				cannon->rotation += 10;
			}
			else if(c == 'g'){
				GameObject *missile = CreateGameObject(&rocketImage);
				missile->position = cannon->position;
				missile->velocity.x = (10.0f*sin(ToRadians(cannon->rotation)));
				missile->velocity.y = -(10.0f*cos(ToRadians(cannon->rotation)));
				missile->type = 2;
				missile->rotation = cannon->rotation;
				VectorAdd(&gameObjects, missile);
			}
	

			else if (c == 'x'){
				break;
			}
			
		}
		int gameObjectCount = VectorTotal(&gameObjects);
		for(i = 0; i < gameObjectCount; i++){
			GameObject* gameObject = VectorGet(&gameObjects, i);
			if(gameObject->type == 2){
				int j;
				for(j = 0; j < gameObjectCount; j++){
					GameObject* gameObject2 = VectorGet(&gameObjects, j);
					if(gameObject2->type == 1){
						if(RectContains(GetGlobalBounds(gameObject2), gameObject->position)){
							
							GameObject* parachute = CreateGameObject(&parachuteImage);
							parachute->position = gameObject2->position;
							parachute->velocity.y = 5;
							parachute->scale = gameObject2->scale/2;
							parachute->type = 3;
							gameObject2->type = 999;
							gameObject->type = 999;
							VectorAdd(&gameObjects, parachute);
							int k;
							for(k = 0; k < VectorTotal(&gameObject2->childs); k++){
								GameObject* child = VectorGet(&gameObject2->childs, k);
								child->acceleration.y = 4;
							}
							GameObject* wheel1 = CreateGameObject(&wheelImage);
							GameObject* wheel2 = CreateGameObject(&wheelImage);
							wheel1->position = gameObject2->position;
							wheel2->position = gameObject2->position;
							wheel1->velocity.x = ((rand()%5) + 5);
							wheel2->velocity.x = -((rand()%5) + 5);
							wheel1->velocity.y = ((rand()%3) - 1);
							wheel2->velocity.y = ((rand()%3) - 1);
							wheel1->type = 4;
							wheel2->type = 4;
							wheel1->scale = gameObject2->scale;
							wheel2->scale = gameObject2->scale;
							wheel1->acceleration.y = 4;
							wheel2->acceleration.y = 4;
							VectorAdd(&gameObjects, wheel1);
							VectorAdd(&gameObjects, wheel2);
							DetachAllChilds(gameObject);
							DetachParent(gameObject);
						}
					}
				}
			}
			//printf("%d ", gameObject->position.x);
		}
		ClearScreen();
		DrawImage(width/2, height/2, &backgroundImage, 1.0f, 0);
		for(i = 0; i < VectorTotal(&gameObjects); i++){
			GameObject* gameObject = VectorGet(&gameObjects, i);
			UpdateGameObject(gameObject);
			Rect boundary = (Rect){-100, -100, width+200, height+200};
			if(!RectContains(boundary, gameObject->position)){
				gameObject->type = 999;
			}
			if(gameObject->type == 3){
				gameObject->rotation = rot;
			} else if (gameObject->type == 4){
				if(gameObject->position.y > height) 
					gameObject->velocity.y = -gameObject->velocity.y * 0.8;
			}
			DrawGameObject(gameObject);
		}
		
		SwapBuffers();
		for(i = 0; i < VectorTotal(&gameObjects); i++){
			GameObject* gameObject = VectorGet(&gameObjects, i);
			if(gameObject->type == 999){
				VectorDelete(&gameObjects, i);
				free(gameObject);
				i--;
			}
		}

		usleep(33333);
	}
	for(i = 0; i < VectorTotal(&gameObjects); i++){
		GameObject* gameObject = VectorGet(&gameObjects, i);
		
		VectorDelete(&gameObjects, i);
		free(gameObject);
	}
	VectorFree(&gameObjects);
	FreeImage(&parachuteImage);
	FreeImage(&backgroundImage);
	FreeImage(&planeImage);
	FreeImage(&propellerImage);
	FreeImage(&wheelImage);
	CloseFramebuffer();
	return 0;
}
