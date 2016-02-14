#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <pthread.h>
#include "font8x8_basic.h"
 
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} Color32;

int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0, *bbp = 0;
int x = 0, y = 0;
long int location = 0;
int shipX = 100;
int quit = 0;

int hit = 0;

typedef struct {
   int x;
   int y;
}pointDirection;
pointDirection point[7] = {(pointDirection){2,0},(pointDirection){148,0},(pointDirection){298,0},(pointDirection){448,0},(pointDirection){598,0},(pointDirection){748,0},(pointDirection){780,0}};


int directionDefault = 3;
void changeDirection(int direction, Color32 color) {
  int pointStartX = 448;
  int pointStartY = 492;
  lineBorder(pointStartX,pointStartY,point[direction].x,point[direction].y, color);
}
 
void setPixel(int x, int y, Color32 color){
  location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
  if(location >= 0 && location < screensize){
    *((uint32_t*)(fbp + location)) = (color.r<<vinfo.red.offset) | (color.g<<vinfo.green.offset) | (color.b<<vinfo.blue.offset) | (color.a<<vinfo.transp.offset);
  } else {
    //Out of bound
  }
}
 
void drawChar(char c, int x, int y, int scale, Color32 color){
  int i,j,a,b;
  int set;
  for (j=0; j < 8; j++) {
    for (i=0; i < 8; i++) {
      set = font8x8_basic[(int)c][j] & 1 << i;
      if(set){
        for(b=0; b<scale; b++){
          for(a=0; a<scale; a++){
            setPixel(x+i*scale+a, y+j*scale+b, color);
          }
        }
      }
    }
  }
}
 
void drawString(char* str, int x, int y, int scale, Color32 color){
  int i = 0;
  while(*str){
    char c = *str++;
    drawChar(c, x+(i*8*scale), y, scale, color);
    i++;
  }
}
 
void drawRectangle(int startX, int startY, int width, int height, int red, int green, int blue){
  for (y = startY; y < startY + height; y++){
    for (x = startX; x < startX + width; x++) {
      location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
             (y+vinfo.yoffset) * finfo.line_length;
 
      if (vinfo.bits_per_pixel == 32) {
        *(fbp + location) = blue;        // Some blue
        *(fbp + location + 1) = green;     // A little green
        *(fbp + location + 2) = red;    // A lot of red
        *(fbp + location + 3) = 0;      // No transparency
    //location += 4;
      } else  { //assume 16bpp
        int b = blue;
        int g = green;     // A little green
        int r = red;    // A lot of red
        unsigned short int t = r<<11 | g << 5 | b;
        *((unsigned short int*)(fbp + location)) = t;
      }
    }
  }
}

//draw line
void line(int x0, int y0, int x1, int y1, Color32 color) {

  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
  
  for(;;){
      setPixel(x0,y0,color);
      if (x0==x1 && y0==y1) break;
      e2 = err;
      if (e2 >-dx) { err -= dy; x0 += sx; }
      if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void lineBorder(int x0, int y0, int x1, int y1, Color32 color) {
  int count = 0;
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
  
  for(;;){
      if (count <= 30) {
        setPixel(x0,y0,color);
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
      }
      else
    break;
      count++;
  }
}

void drawBullet(int x0, int y0, int x1, int y1, Color32 color) {
  int count = 0;
  int size = 50;
  int start[2];
  int end[2];
  Color32 black = {0,0,0,0};
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
  int pointMem[50][2];
  for(;;){
    pointMem[count][0] = x0;
    pointMem[count][1] = y0;
  
    if(count<size){
      setPixel(x0,y0,color);
      if ((x0 >= shipX) && (x0 <= shipX + 41*8) && (y0 >= 0) && (y0 <= 56)){
        int k = 0;
        hit = 1;
        for(k=0; k<count; k++)
          setPixel(pointMem[k][0],pointMem[k][1],(Color32){0,0,0,0});  
          break;
      }
      if (x0==x1 && y0==y1) {
        int k = 0;
        for(k=0; k<count; k++)
          setPixel(pointMem[k][0],pointMem[k][1],(Color32){0,0,0,0});  
          break;
      }
      e2 = err;
      if (e2 >-dx) { err -= dy; x0 += sx; }
      if (e2 < dy) { err += dx; y0 += sy; }
      count++;
    }else{
      usleep(100000);
      int k = 0;
      for(k=0; k<50; k++)
    setPixel(pointMem[k][0],pointMem[k][1],(Color32){0,0,0,0});
      count = 0;
    }
  }
}

void drawShip(int x, Color32 color){
  drawString("              __    __    __ ", x, 0, 1, color);
  drawString("             |==|  |==|  |==| ", x, 8, 1, color);
  drawString("           __|__|__|__|__|__|_ ", x, 16, 1, color);
  drawString("        __|___________________|___ ", x, 24, 1, color);
  drawString("     __|__[]__[]__[]__[]__[]__[]__|___ ", x, 32, 1, color);
  drawString("    |............................o.../ ", x, 40, 1, color);
  drawString("    \\.............................../ ", x, 48, 1, color);
  drawString("_,~')_,~')_,~')_,~')_,~')_,~')_,~')/,~')_", x, 56, 1, color);
  if (hit) {
    color = (Color32){0,0,0,0};
    drawString("              __    __    __ ", x, 0, 1, color);
    drawString("             |==|  |==|  |==| ", x, 8, 1, color);
    drawString("           __|__|__|__|__|__|_ ", x, 16, 1, color);
    drawString("        __|___________________|___ ", x, 24, 1, color);
    drawString("     __|__[]__[]__[]__[]__[]__[]__|___ ", x, 32, 1, color);
    drawString("    |............................o.../ ", x, 40, 1, color);
    drawString("    \\.............................../ ", x, 48, 1, color);
    drawString("_,~')_,~')_,~')_,~')_,~')_,~')_,~')/,~')_", x, 56, 1, color);
    // usleep(150000);
    // color = (Color32){255,255,255,0};
    // hit = 0;
  }
}

void drawJet(int x, Color32 color){
  drawString("     /\\      ", x, 500, 1, color);
  drawString("    /<>\\     ", x, 508, 1, color);
  drawString("   /=  =\\    ", x, 516, 1, color);
  drawString("  /      \\   ", x, 524, 1, color);
  drawString(" / /\\  /\\ \\  ", x, 532, 1, color);
  drawString("/ /  \\/  \\ \\ ", x, 540, 1, color);
  drawString("\\/   /\\   \\/", x, 548, 1, color);
}

void moveShip(){
  int moveRight = 1;

  for(;;){
    for (;moveRight;){
        usleep(100000);
        drawShip(shipX, (Color32){0,0,0,0});
        
        shipX = shipX + 20;
        if (shipX + 390 > vinfo.xres) {moveRight = 0;break;}
        drawShip(shipX, (Color32){255,255,255,0});
        if(quit)
      break;
      } 
    if(quit)
      break;
    
      for (;!moveRight;){
        usleep(100000);
        drawShip(shipX, (Color32){0,0,0,0});
        
        shipX = shipX - 20;
        if (shipX <= 0) {moveRight = 1;break;}
        drawShip(shipX, (Color32){255,255,255,0});
        if(quit)
      break;
      }
      if(quit)
       break;
  }
  
}

void moveShip2(){
  int moveRight =1;
  drawShip(shipX, (Color32){255,255,255,0});
  shipX += moveRight ? 20 : -20;
  if(shipX + 390 > vinfo.xres) {moveRight = 0;}
  else if (shipX <= 0) {moveRight = 1;}
}

//shoot function

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

void *shoot()
{
    set_conio_terminal_mode();
  while(1){
    while (!kbhit()) {
      //wait
    }
    
    int key = getch(); //get key pressed
    Color32 color = (Color32){100,100,100,0};
    Color32 bulletColor = (Color32){255,0,0,0};
    //printf("%d\n",key);
    if(key == 32){ //spacebar
      //shoot  
      drawBullet(448,492,point[directionDefault].x,point[directionDefault].y,bulletColor);
      changeDirection(directionDefault, (Color32){255,255,255,0});
    }
    else if(key == 97 || key == 65){ // a
      // move left
      if(directionDefault > 0){
      changeDirection(directionDefault, (Color32) {0,0,0,0});
      directionDefault--;
      changeDirection(directionDefault, (Color32){255,255,255,0});
    }
    }
    else if(key == 100 || key == 68){ //d
      //move right
      if(directionDefault < 6){
      changeDirection(directionDefault, (Color32) {0,0,0,0});
      directionDefault++;
      changeDirection(directionDefault, (Color32){255,255,255,0});
      }
    }
    else{ //exit
      quit = 1; //kill all threads
      break;
    }
  }
}

//ship thread
void *ship(){
  moveShip();
}

int main()
{
  // Open the file for reading and writing
  fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1) {
    perror("Error: cannot open framebuffer device");
    exit(1);
  }
  printf("The framebuffer device was opened successfully.\n");
 
  // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
    perror("Error reading fixed information");
    exit(2);
  }
 
  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
    perror("Error reading variable information");
    exit(3);
  }
 
  printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
 
  // Figure out the size of the screen in bytes
  screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
 
  // Map the device to memory
  fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
  if ((int)fbp == -1) {
    perror("Error: failed to map framebuffer device to memory");
    exit(4);
  }
  printf("The framebuffer device was mapped to memory successfully.\n");
  
  // Clear Screen
  drawRectangle(0, 0, vinfo.xres, vinfo.yres, 0, 0, 0);
  
  //ship thread
  pthread_t ship_thread;
  
  if(pthread_create(&ship_thread, NULL, ship,NULL)){
    printf("ship thread error\n");
    return 1;
  }
  
  //create thread for shoot
  pthread_t shoot_thread;
  
  if(pthread_create(&shoot_thread, NULL, shoot,NULL)){
    printf("shoot thread error\n");
    return 1;
  }
  
  //set color
  Color32 color = (Color32){255,255,255,0};
  //drawBullet(448,492,600,600,color);
  drawJet(400, color);
  changeDirection(directionDefault, color);
  
  //thread join all
  if(pthread_join(shoot_thread, NULL)) {
    fprintf(stderr, "Error joining shoot thread\n");
    return 2;
  }
  if(pthread_join(ship_thread, NULL)) {
    fprintf(stderr, "Error joining ship thread\n");
    return 2;
  }
  
  // Clear Screen
  drawRectangle(0, 0, vinfo.xres, vinfo.yres, 0, 0, 0);
  
  return 0;
}
