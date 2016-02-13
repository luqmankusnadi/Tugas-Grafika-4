#include "image.h"

void AllocImage(Image *image, int width, int height){
	image->pixels = malloc(height*sizeof(Color32*));
	image->w = width;
	image->h = height;
	image->pivot.x = width/2;
	image->pivot.y = height/2;
	if (image->pixels)
	{
		int i;
		for (i = 0; i < height; i++)
		{
			image->pixels[i] = malloc(sizeof(Color32) * width);
		}
	}
}
void FreeImage(Image *image){
	int i;
	for(i = 0; i < image->h; i++){
		free(image->pixels[i]);
	}
	free(image->pixels);
}
