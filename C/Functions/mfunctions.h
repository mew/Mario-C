#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Prototypes/mprototypes.h"


#ifndef FUNCTIONS
void updatePosition(struct velocity *modifier, SDL_Rect *position, SDL_Rect *dummy, int gGrid[GRIDY][GRIDX], int inAir){ //update mario y
	
	SDL_Rect test;
	int i;
	int broken = 0;
	
	//printf("PosX1: %lf", position->x);
	//position->x += /*((position->x > -1*modifier->xa && position->x < (678-abs(modifier->xa))) ?*/ modifier->xa /*: 0)*/;
	DEBUG_PRINT(("modX:%lf PosX2: %lf\n", modifier->xa, position->x));
	
	if(modifier->ya >= 0){

		for(i=0;i<=modifier->ya;i++){

			//printf("%d, %d\n", position->y/28, (dummy->x+position->x+i-42));

			if(isBottomSolid(gGrid[(position->y-i)/28][(dummy->x+position->x)/28]) || isBottomSolid(gGrid[(position->y-i)/28][((dummy->x+position->x)/28) + 1])){
								
				position->y -= (i-1);
				broken = 1;
				modifier->ya = 0;
				printf("Head smash!\n");
				break;
			}

		}

		if(!broken){
			position->y -= modifier->ya;
		}

	} else {

	position->y -= modifier->ya;

	}

	test.x = position->x;
	test.y = position->y;

	//printf("posY:%d\n", test.y);
	//printf("calculated gGrid y:%d\n", (test.y/28)+1);
	//printf("block id

	if(onGround(&test, dummy, gGrid) && modifier->ya <= 0 && inAir /*&& gGrid[(test.y/28) + 1][test.x/28] == 0*/){
	
		snapToGrid(position, dummy);
		inAir = 0;
		DEBUG_PRINT(("Snapping!\n"));
		printf("Snapping!\n");

	}
	//position->y = (((modifier->ya < 0) && (position->y - modifier->ya > )) ?  586 : (position->y - modifier->ya));
}

void doGravity(struct velocity *toGravity){ //gravity application
	if(toGravity->ya > TERMINAL){	
		toGravity->ya -= 1.0;
	}
}

void doDirection(struct velocity *dirCheck, int *direction){ //check and update direction
	if(dirCheck->xa == 0){
		*direction = 0;
	} else {
		*direction = ((dirCheck->xa > 0) ? 1 : -1);
	}		
}

int onGround(SDL_Rect *position, SDL_Rect *dummy_rect, int gGrid[GRIDY][GRIDX]){

	int x1, x2, x3;
	int y;
	x1 = ((((int)position->x) + dummy_rect->x)/28);
	x2 = ((((int)position->x)+ MARIOX + dummy_rect->x)/28);
	//x3 = ((((int)position->x)+ 21 + dummy_rect.x)/28);
	y = ((((int)position->y))/28) + 2;
	printf("x:%d, y:%d coord:%d\n", x1, y, gGrid[y][x1]);

	return ((isTopSolid(gGrid[y][x1]) || isTopSolid(gGrid[y][x2])) ? 1 : 0);

}


void snapToGrid(SDL_Rect *position, SDL_Rect *dummy_rect){

	int x;
	int y;
	x = ((((int)position->x) + 21 + dummy_rect->x)/28);
	y = ((((int)position->y))/28) + 2;
	//position->x = x*28;
	position->y = (y*28) - 36;

	printf("PosY:%d\n", position->y);

}

void doScrolling(SDL_Rect *dummy, SDL_Rect *position, struct velocity *modifier, int gGrid[GRIDY][GRIDX]){

	int i;
	int broken = 0;	

	//dummy->x += modifier->xa;

	if(modifier->xa > 0){

		for(i=0;i<=modifier->xa;i++){

			//printf("%d, %d\n", position->y/28, (dummy->x+position->x+i-42));

			if(isSideSolid(gGrid[position->y/28][(dummy->x+position->x+i+MARIOX)/28]) || isSideSolid(gGrid[position->y/28 + 1][(dummy->x+position->x+i+MARIOX)/28])){
								
				dummy->x += (i-1);
				broken = 1;
				modifier->xa = 0;
				//printf("Wall ram!\n");
				break;
			}

		}

		if(!broken){
			dummy->x += modifier->xa;
		}

	} else if(modifier->xa < 0){

		for(i=0;i>=modifier->xa;i--){

			//printf("%d, %d\n", position->y/28, (dummy->x+position->x+i-42));

			if(isSideSolid(gGrid[position->y/28][(dummy->x+position->x+i)/28]) || isSideSolid(gGrid[position->y/28 + 1][(dummy->x+position->x+i)/28])){
								
				dummy->x += (i+1);
				broken = 1;
				modifier->xa = 0;
				//printf("Wall ram!\n");
				break;
			}

		}

		if(!broken){
			dummy->x += modifier->xa;
		}

	} else {

		dummy->x += modifier->xa;

	}

}

int isTopSolid(int tile){

	return ((tile != 0) ? 1 : 0);

}

int isSideSolid(int tile){

	return ((tile == 2 || tile == 5 || tile == 6 || tile == 7 || tile == 8) ? 1 : 0);

}

int isBottomSolid(int tile){

	return ((tile == 2 || tile == 5 || tile == 6 || tile == 7 || tile == 8) ? 1: 0);

}

void doInit(){

	srand(time( NULL ));


	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0){		//initializing SDL components
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if(TTF_Init() < 0){								//initializing TTF
		printf("Couldn't initialize SDL ttf extension: %s\n", SDL_GetError());	
		exit(1);
	}

	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1){			//initializing audio

		printf("couldn't open mixer\n");
		exit(1);
	}

	IMG_Init(IMG_INIT_PNG);

}

void loadBlocks(SDL_Texture *blockpng[], SDL_Renderer *renderer){

	int i;	
	char fname[100];
	SDL_Surface *image;

	for(i=0;i<20;i++){

		sprintf(fname, PATH"/Assets/Images/block%d.png", i);

		if(access(fname, F_OK) != -1){

			image = IMG_Load(fname);
		
		} else {

			image = IMG_Load(PATH"/Assets/Images/missingno.png");

		}

		blockpng[i] = SDL_CreateTextureFromSurface(renderer, image);

		

	}

}

void loadGrid(int gGrid[GRIDY][GRIDX], FILE *fh){

	int i, j, k;
	char throwaway;

	for(i=0;i<GRIDY;i++){

		for(j=0;j<GRIDX;j++){

			fscanf(fh, "%d", &gGrid[i][j]);

		}

		while ((throwaway = fgetc(fh)) != '\n') {
		}
                

	}

}

SDL_Joystick *loadJoysticks(SDL_Joystick *joystick){

	int i;

	if(SDL_NumJoysticks() == 0){
		printf("Found no joysticks!\n");
		printf("Using keypad. Functionality will not be ideal.\n");
		return NULL;
		
	} else {

		printf("%d joysticks were found!\n", SDL_NumJoysticks());

	}

	printf("The names of the joysticks are: \n");
	for(i = 0; i < SDL_NumJoysticks(); i++){

		joystick = SDL_JoystickOpen(i);

		printf(">> %s, %d\n", SDL_JoystickName(joystick), i);

	}

	return joystick;

}

#define FUNCTIONS
#endif


