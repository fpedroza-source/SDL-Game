#pragma once
#define MAX_ANIMATION 16
#define MAX_FRAMES 16
#define SPRITE_SIZE 128
#define STATE_IDLE 0
#define STATE_RUN 1
#define STATE_UPTODUCK 2
#define STATE_DUCKTOUP 3
#define STATE_ROLL 4
#include <SDL3/SDL.h>
#include <stdio.h>

typedef struct {
    int row;
    int col;
    int duration;
    int flip;
    int next_frame;
    int next_state;
    int incx;
    int incy;
} Frame;

typedef struct {
    Frame frames[MAX_FRAMES];
    size_t length;   
    int frame_time;
    size_t frame_index;
    /* data */
} Animation;

typedef struct {
    int posx;
    int posy;
    bool facing;
    size_t current;    
    Animation collection[MAX_ANIMATION];    
} Animations;


SDL_FRect GetSpriteRect(Animation* anim);
bool Animate(Animations* animations);
bool HandleKeyPress(Animations* animatinos); 
bool LoadAnimations(Animations *animations, const char* filename);
