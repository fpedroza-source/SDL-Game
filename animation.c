#include "animation.h"

SDL_FRect GetSpriteRect(Animation* anim)
{
    int col = anim->frames[anim->frame_index].col;
    int row = anim->frames[anim->frame_index].row;

    int x = col * SPRITE_SIZE;
    int y = row * SPRITE_SIZE;
    SDL_FRect rect = {x, y, SPRITE_SIZE, SPRITE_SIZE};
    return rect;
}

bool Animate(Animation* anim, int *cur_state)
{
    
    if (anim->frame_time >= anim->frames[anim->frame_index].duration) {
        int next_frame = anim->frames[anim->frame_index].next_frame;
        int next_state = anim->frames[anim->frame_index].next_state;
        if (next_state >= 0) {
            *cur_state = next_state;
            return true;
        }
        if (next_frame>=0) {
            anim->frame_index = next_frame;
         } else {
            anim->frame_index++;
            if (anim->frame_index >= anim->length) anim->frame_index = 0;
        }
        anim->frame_time = -1;        
    }
    anim->frame_time++;
    return false;
};

bool LoadAnimations(Animations *animations, const char* filename)
{
    int length = 0;
    int total = 0;
    int index = -1;
    int col = 0;
    int row = 0;
    int duration = 0;
    int flip = 0;
    double angle = 0;
    int next_frame = -1;
    int next_state = -1;
    int frame_index = 0;
    char text[80];

    FILE *file = fopen(filename, "r");
        
    if (file == NULL) return false;

     for (int i =0; i < MAX_ANIMATION; i++) {                
       animations->collection[i].frame_time = 0;
       animations->collection[i].frame_index = 0;
     }

    while (fscanf(file, "%s", text) != EOF) {
    
        if (strcmp(text, "length:") == 0) {
            index++;
            fscanf(file, "%d", &length);
            animations->collection[index].length = length;                                          
        } else if (strcmp(text, "frame:") == 0) {
            fscanf(file, "%d", &frame_index);            
        } else if (strcmp(text, "col:") == 0) {
            fscanf(file, "%d", &col);            
        } else if (strcmp(text, "row:") == 0) {
            fscanf(file, "%d", &row);            
        } else if (strcmp(text, "duration:") == 0) {
            fscanf(file, "%d", &duration);            
        } else if (strcmp(text, "flip:") == 0) {
            fscanf(file, "%d", &flip);  
        } else if (strcmp(text, "nextframe:") == 0) {
            fscanf(file, "%d", &next_frame);  
        } else if (strcmp(text, "nextstate:") == 0) {
            fscanf(file, "%d", &next_state);  
        } else if (strcmp(text, "angle:") == 0) {
            fscanf(file, "%lf", &angle);
        }
        if (index >=0) {
            animations->collection[index].frames[frame_index].col = col;
            animations->collection[index].frames[frame_index].row = row;
            animations->collection[index].frames[frame_index].duration = duration;
            animations->collection[index].frames[frame_index].flip = flip;
            animations->collection[index].frames[frame_index].angle = angle;
            animations->collection[index].frames[frame_index].next_frame = next_frame;
            animations->collection[index].frames[frame_index].next_state = next_state;
        }       
    }
    fclose(file);
    return true;   
}

bool HandleKeyPress(Animation* current, int* state) {
    const bool* keys = NULL;
    keys = SDL_GetKeyboardState(NULL);
    if (keys !=NULL)
    {
        switch (*state)
        {
        case STATE_IDLE:
            /* code */
            if (keys[SDL_SCANCODE_DOWN]) {
                *state = STATE_UPTODUCK;
                return true;
            } else if (keys[SDL_SCANCODE_RIGHT]) {
                *state = STATE_RUN;
                return true;
            } else if (keys[SDL_SCANCODE_SPACE]) {
                *state = STATE_ROLL;
                return true;
            }
            break;
        case STATE_RUN:
            if (!keys[SDL_SCANCODE_RIGHT]) {
                *state = STATE_IDLE;
                return true;
                }                             
            break;
        case STATE_UPTODUCK:
            if (!keys[SDL_SCANCODE_DOWN]) {
                *state = STATE_DUCKTOUP;
                return true;
                }                             
            break;
        
        default:
            break;
        }
    }
    return false;


}