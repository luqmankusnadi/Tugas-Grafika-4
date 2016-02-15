all:
	gcc -o exec main.c bitmap.c image.c gameobject.c input.c framebuffer.c vector.c rect.c timer.c -lm
    
run:
	sudo ./exec
