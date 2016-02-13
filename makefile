all:
	gcc -o exec main.c bitmap.c image.c gameobject.c input.c framebuffer.c -lm
    
run:
	sudo ./exec
