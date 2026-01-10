#pragma once
#include "animation.h"


void DrawMap(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* mapgrid, const int index);
void DrawObj(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* objects, const int index);
int CheckMapCollision(Animations* mapgrid, SDL_FRect *frame_rect, const int index);
int CheckObjCollision(Animations* objgrid, SDL_FRect *frame_rect, const int index);