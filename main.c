#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "framebuffer.h"
#include "bitmap.h"
#include "input.h"


//Function untuk missile
int missile_finished_launched(int x,int y,int width_background){	
	return y < 0 || x < 0 || x > width_background;
}

//screen size
long int width, height;

int main(){	
	InitFramebuffer(&width, &height);
	// Image spaceshipImage = LoadBitmapAsImage("spaceship.bmp");
	Image parachuteImage = LoadBitmapAsImage("parachute.bmp");
	parachuteImage.pivot.y = 0;
	Image backgroundImage = LoadBitmapAsImage("background.bmp");
	Image rocketImage = LoadBitmapAsImage("rocket.bmp");
	Image missileImage = LoadBitmapAsImage("rocket.bmp");
	Image planeImage = LoadBitmapAsImage("plane.bmp");
	Image propellerImage = LoadBitmapAsImage("propeller.bmp");

	int rot = 0;
	int rocket_rot=0;
	int missile_rot = 0;
	int rot_offset = 6;
	int rocket_offset = 6;
	int rot_max = 50;
	int y_parachute = 300;
	int y_parachute2 = 300;
	int y_parachute_offset = 10;
	int i;
	
	//Plane vars
	int plane_x = width/2 -20;
	int plane_y = height/2 -35;
	float plane_scale = 1.0;
	int plane_rot = 0;
	int isPlaneFall = 0;
	
	//Propeller vars
	int propeller_rot = 0;
	int propeller_rot_delta = 35;
	int propeller_x_delta = 30;
	int propeller_y_delta = 30;
	float propeller_scale = 1.0;
	
	//Missile Variable
	int missile_launched = 0;
	int missile_offset = 15;
	int missile_x = backgroundImage.w/2;
	int missile_y = backgroundImage.h;
	float missile_scale = 0;
	int missile_rotation = 0;

	set_conio_terminal_mode();

	for(i=0; i < 10000; i++){
		ClearScreen();
		
		rot = ((rot + rot_offset)% rot_max);
		if (rot_max - rot < 3){
			rot_offset *=  -1;
		}
		else if (rot_max - rot > 97){
			rot_offset *=  -1;
		}
		
		DrawImage(width/2, height/2, &backgroundImage, 1.0f, 0);
		DrawImage(300,y_parachute += y_parachute_offset, &parachuteImage, 0.5f, rot);
		DrawImage(backgroundImage.w/2, backgroundImage.h, &rocketImage, 1.0f, rocket_rot);
		if(i>20)
			isPlaneFall = 1;
		//draw plane
		if(!isPlaneFall){
			DrawImage(plane_x, plane_y, &planeImage, plane_scale, plane_rot);
			plane_scale += 0.03;

			propeller_rot+=propeller_rot_delta;
			propeller_rot %= 360;
			propeller_scale += 0.03;

			//draw propeller
			DrawImage(width/2, height/2, &propellerImage, propeller_scale, propeller_rot);
		}
		else{
			DrawImage(plane_x, plane_y, &planeImage, plane_scale, plane_rot);
			plane_y += 8;
			plane_rot = (plane_rot + 2) % 360;

			if (propeller_rot_delta) propeller_rot_delta-=1;
			propeller_rot+=propeller_rot_delta;
			propeller_rot %= 360;
			propeller_x_delta+=6;
			propeller_y_delta+=6;

			//draw propeller
			DrawImage(plane_x + propeller_x_delta, plane_y + propeller_y_delta, &propellerImage, propeller_scale, propeller_rot);
		}
		
		if (missile_launched == 1){
			DrawImage(missile_x += missile_rot,missile_y -= missile_offset, &rocketImage, 1.0f, missile_rot);			
		}
		if (missile_finished_launched(missile_x,missile_y,backgroundImage.w) ){
			missile_x = backgroundImage.w/2;
			missile_y = backgroundImage.h;
			missile_launched = 0;
			missile_rot = rocket_rot;
		}

		DrawLine(0,0, 700, 600, (Color32){255,0,0,255});
		if(kbhit()){
			char c = getch();
			if(c == 'a'){
				if (rot_max - rocket_rot < 3){
				}
				else{
					rocket_rot = ((rocket_rot - rocket_offset)% rot_max);
				}

			} 
			else if(c == 'd'){
				printf("Kanan");
				printf("%d", rocket_rot);
				if (rot_max - rocket_rot > 97){
				}else{
					rocket_rot = ((rocket_rot + rocket_offset)% rot_max);
				}

			}
			else if(c == 'z'){
				missile_launched = 1;
			}

			else if (c == 'x'){
				break;
			}
		}
		SwapBuffers();
		usleep(33333);
	}
	
	//free image
	FreeImage(&parachuteImage);
	FreeImage(&backgroundImage);
	FreeImage(&planeImage);
	FreeImage(&propellerImage);
	CloseFramebuffer();
	return 0;
}
