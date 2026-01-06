#pragma once
#include "animation.h"
#define MAPTILES 0
#define MAPCOORDS 1

typedef struct {
    SDL_FRect coord[MAX_ANIMATION];

} MapConstrains;

//MapConstrains mapcoord;

void DrawMap(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* mapgrid, int index);
int CheckMapCollision(SDL_Renderer *renderer, Animations* mapgrid, SDL_FRect *frame_rect, int index);