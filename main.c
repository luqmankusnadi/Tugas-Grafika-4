#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "framebuffer.h"
#include "bitmap.h"

int main(){
	
	InitFramebuffer();
	// Image spaceshipImage = LoadBitmapAsImage("spaceship.bmp");
	Image parachuteImage = LoadBitmapAsImage("parachute.bmp");
	parachuteImage.pivot.y = 0;
	Image backgroundImage = LoadBitmapAsImage("background.bmp");

	int rot = 0;
	int i;
	
	for(i=0; i < 10000; i++){
		ClearScreen();
		rot = ((rot+13)%361);
		DrawImage(1024/2, 768/2, &backgroundImage, 1.0f, 0);
		DrawImage(300,300, &parachuteImage, (float)i/100.0f, rot);
		DrawLine(0,0, 700, 600, (Color32){255,0,0,255});
		SwapBuffers();
		
		usleep(33333);
	}
	FreeImage(&parachuteImage);
	FreeImage(&backgroundImage);
	CloseFramebuffer();
	return 0;
}
