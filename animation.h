#pragma once
#define SPRITE_SIZE 128
#define STATE_IDLE 0
#define STATE_RUN 1
#define STATE_UPTODUCK 2
#define STATE_DUCKTOUP 3
#include <SDL3/SDL.h>
#include <stdio.h>

typedef struct 
{
    int row;
    int col;
    int duration;
    int flip;
    int next_frame;
    int next_state;
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


SDL_FRect GetSpriteRect(Animation* anim);
bool Animate(Animation *anim, int *state);
bool HandleKeyPress(Animation* current, int* state); 
bool LoadAnimations(Animations *animations, const char* filename);
