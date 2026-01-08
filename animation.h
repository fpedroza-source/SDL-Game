#pragma once
#define MAX_ANIMATION 16
#define MAX_FRAMES 16
#define STATE_IDLE 0
#define STATE_RUN 1
#define STATE_UPTODUCK 2
#define STATE_DUCKTOUP 3
#define STATE_TURN 4
#define STATE_ROLL 5
#define STATE_ATTACK1 6
#define STATE_FALLING 7

#define WINDOW_W 800
#define WINDOW_H 600

#include <SDL3/SDL.h>
#include <stdio.h>

typedef struct {
    int duration;
  //  int flip;
    int next_frame;
    int next_state;    
    SDL_FPoint incxy;
    SDL_FPoint posxy;
    SDL_FRect box;
    SDL_FRect colbox;
} Frame;

typedef struct {
    Frame frames[MAX_FRAMES];
    size_t length;   
    int frame_time;
    int frame_index;
} Animation;

typedef struct {
    SDL_FPoint pos;
    bool facing;
    size_t current;    
    char filename[80];
    Animation collection[MAX_ANIMATION];    
} Animations;


bool Animate(Animations* animations);
bool HandleKeyPress(Animations* animatinos); 
bool LoadAnimations(Animations *animations, const char* filename);