#include "../../Definitions/mdefines.h"
#include "../../Structs/mstructs.h"

#ifndef PROTOTYPES
void updatePosition(struct velocity *modifier, SDL_Rect *position, SDL_Rect *dummy, int gGrid[GRIDY][GRIDX], int inAir);
void doGravity(struct velocity *toGravity);
void doDirection(struct velocity *dirCheck, int *direction);
int onGround(SDL_Rect *position, SDL_Rect *dummy_rect, int gGrid[GRIDY][GRIDX]);
void snapToGrid(SDL_Rect *position, SDL_Rect *dummy_rect);
void doScrolling(SDL_Rect *dummy, SDL_Rect *position, struct velocity *modifier, int gGrid[GRIDY][GRIDX]);
int isTopSolid(int tile);
int isSideSolid(int tile);
int isBottomSolid(int tile);
void doInit();
void loadGrid(int gGrid[GRIDY][GRIDX], FILE *fh);
SDL_Joystick *loadJoysticks(SDL_Joystick *joystick);
#define PROTOTYPES
#endif
