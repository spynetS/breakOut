#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "../printer/src/Canvas.h"
#include "../sound/sound.h"
#include "keymanager.h"

#define RECTW 4
#define RECTH 5
Canvas *canvas;

Rect *rectangles[RECTH*RECTW];

int insideRect(Rect *rect1, Pixel pixel, int ax, int ay){
	
	Rect rect2;
	rect2.x = pixel.x+ax;
	rect2.y = pixel.y+ay;
	rect2.w = 1;
	rect2.h = 1;

	if (
    rect1->x < rect2.x + rect2.w &&
    rect1->x + rect1->w > rect2.x &&
    rect1->y < rect2.y + rect2.h &&
    rect1->y + rect1->h > rect2.y
  ) {
		return 1;
  } else {
		return 0;
  }

	return 0;
}

void spawnRects(){
	int w = canvas->width/RECTW;	
	int x = 0;
	int y = 0;
	int color = 0;
	for(int i = 0; i <= RECTH*RECTW; i ++){
		Rect *rect = malloc(sizeof(Rect));
	

		rect->w = w;
		rect->h = 1;

		// rect->x = (i ) * rect->w;
		// rect->y = y;
		rect->x = x*w;
		rect->y = y; 
		
		if(x >= canvas->width/w){
			x=-1;
			y++;
		}
		x++;
		rect->ch = "■ ";
		
		if(color == 6) color = -1;
		color ++;
		switch(color){
			case 0:
				rect->color = BLUE;
				break;
			case 1:
				rect->color = GREEN;
				break;
			case 2:
				rect->color = RED;
				break;
			case 3:
				rect->color = PURPLE;
				break;
			case 4:
				rect->color = CYAN;
				break;
			case 5:
				rect->color = YELLOW;
				break;
			case 6:
				rect->color = BLUE;
				break;
		}

		rect->bgcolor = "";
		rectangles[i] = rect;
	}
}

int main(){
	canvas = newCanvas(20,20,". ",BLACK,"");
	canvas->x = termWidth()/2-canvas->width;

    spawnRects();
		pthread_t cThread;
		if(pthread_create(&cThread, NULL, startKeyListener, NULL)){
			perror("ERROR creating thread.");
		}

   
    Rect *player = malloc(sizeof(Rect));
    player->x = 0;
    player->y = canvas->height-3;
    player->w = 5;
    player->h = 2;
	player->ch = "■";
	player->color = WHITE;
	player->bgcolor = BG_BLACK;
    
    
    Pixel *ball = newPixel(0,4, "● ", "", "");
    //accelaration values
    int ax = 1;
    int ay = 1;
    
    int frames = 0;



    while(1){
    	
    	clearPixels(canvas);
    
    	setBorder(canvas,0);
		char* key = malloc(sizeof(char*));
		sprintf(key,"%d",down);
		setText(canvas,0,6,key,"","");
    	
    	// char key = getKeyPressed();
    	if(down == 32) player->x ++;
    	if(down == 30) player->x --;
    	for(int i = 0; i < RECTW*RECTH; i ++){
    		setRect(canvas, rectangles[i]);
    		if(insideRect(rectangles[i], *ball, ax, ay) == 1){
    			ay = 1;
    			rectangles[i]->y = 100;
    			BeepThread(520,200); }
    	}
    
    	setRect(canvas,player);
    	setPixelWithPixel(canvas, ball);
    
    		ball->x += ax;
    		ball->y += ay;
    
    	if(insideRect(player, *ball, ax, ay) == 1){
    		BeepThread(420,200);
    		ay = -1;
    	}
    	if(ball->x >= canvas->width-2){
    		ax = -1;
    	}
    	if(ball->x <= 0){
    		ax = 1;
    	}
		if(ball->y > canvas->height){
			exit(0);
		}
    	draw(canvas);
    	msleep(100);
    }
    
    free(ball);
    free(player);
	freeCanvas(canvas);

	return 0;
}
