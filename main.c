#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "framebuffer.h"
#include "bitmap.h"

int main(){
	
	InitFramebuffer();
	Image spaceshipImage = LoadBitmapAsImage("spaceship.bmp");
	Image backgroundImage = LoadBitmapAsImage("background.bmp");
	int rot = 0;
	int i;
	
	for(i=0; i < 10000; i++){
		ClearScreen();
		rot = ((rot+10)%360);
		DrawImage(1024/2, 768/2, &backgroundImage, 1.0f, 0);
		DrawImage(300,300, &spaceshipImage, (float)i/100.0f, rot);
		DrawLine(0,0, 700, 600, (Color32){255,0,0,255});
		SwapBuffers();
		usleep(33333);
	}
	FreeImage(&spaceshipImage);
	FreeImage(&backgroundImage);
	CloseFramebuffer();
	return 0;
}
