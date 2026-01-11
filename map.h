#pragma once
#include "animation.h"


void DrawMap(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* mapgrid, const float map_pos);
void DrawObj(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* objects, const float map_pos);
int CheckMapCollision(Animations* mapgrid, Animations* hero, SDL_FRect* result_rect);
enum Objects CheckObjCollision(Animations* objgrid, Animations* hero, SDL_FRect* result_rect);