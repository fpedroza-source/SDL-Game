#pragma once
#define MAX_ANIMATION 16
#define MAX_FRAMES 16
#define WINDOW_W 800
#define WINDOW_H 600

#include <SDL3/SDL.h>
#include <stdio.h>

enum States {
    STATE_IDLE, 
    STATE_RUN, 
    STATE_UPTODUCK, 
    STATE_DUCKTOUP, 
    STATE_TURN, 
    STATE_ROLL, 
    STATE_ATTACK1, 
    STATE_FALLING,
    STATE_CLIMBING};


typedef struct {
    int duration;
  //  int flip;
    int next_frame;
    enum States next_state;    
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
    enum States current;    
    char filename[80];
    Animation collection[MAX_ANIMATION];    
} Animations;


bool Animate(Animations* animations);
bool HandleKeyPress(Animations* animatinos); 
bool LoadAnimations(Animations *animations, const char* filename);