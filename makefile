all:
	gcc -o exec main.c bitmap.c image.c framebuffer.c -lm
    
run:
	sudo ./exec
