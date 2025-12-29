#pragma once
#define SPRITE_SIZE 128
#include <SDL3/SDL.h>
#include <stdio.h>

#define STATE_IDLE 0
#define STATE_RUN 1


typedef struct 
{
    int row;
    int col;
    int duration;
} Frame;

typedef struct 
{
    Frame* frames;
    int length;   
    int frame_time;
    uint64_t frame_index;
    /* data */
} Animation;

typedef struct Animations
{
    Animation *collection;
    int total;
} Animations;


SDL_FRect GetSpriteRect(int col, int row, int width, int height);
SDL_FRect Animate(Animation *anim);
bool LoadAnimations(Animations *animations, const char* filename);