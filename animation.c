#include "animation.h"
#define READATRRIBUTE(a, v) if (strcmp(text, a) == 0) {fscanf(file, "%d", &v);}

bool Animate(Animations* animations)
{
    if (HandleKeyPress(animations))
    {
        animations->collection[animations->current].frame_index = 0;
        animations->collection[animations->current].frame_time = 0;
    }
    Animation* anim = &animations->collection[animations->current];

    int incx = anim->frames[anim->frame_index].incx;
    int incy = anim->frames[anim->frame_index].incy;
    incx -= (animations->facing*incx)*2;
    animations->pos.x += incx;
    animations->pos.y += incy;
    if (animations->pos.x < 0) animations->pos.x = 0;

    if (anim->frame_time >= anim->frames[anim->frame_index].duration) {
        int next_frame = anim->frames[anim->frame_index].next_frame;
        int next_state = anim->frames[anim->frame_index].next_state;
        if (next_state >= 0) {
            animations->current = next_state;
            animations->collection[next_state].frame_index = 0;
            animations->collection[next_state].frame_time = 0;
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
    size_t length = 0;    
    int index = -1;   
    int duration = 0;
    int next_frame = -1;
    int next_state = -1;
    int frame_index = 0;
    int incx = 0;
    int incy = 0;
    int boxx = 0,boxy = 0,boxw = 0,boxh = 0;
    int colboxx = 0, colboxy = 0, colboxw = 0, colboxh = 0;
    char text[80];

    FILE *file = fopen(filename, "r");
        
    if (file == NULL) return false;

    SDL_memset(animations, 0, sizeof(*animations));

    while (fscanf(file, "%s", text) != EOF) {
    
        if (strcmp(text, "filename:") == 0) {
            fscanf (file, "%s", animations->filename);
        } else if (strcmp(text, "length:") == 0) {
            index++;
            fscanf(file, "%llu", &length);
            animations->collection[index].length = length;                                          
        } else READATRRIBUTE("frame:", frame_index)       
        else READATRRIBUTE("duration:", duration)    
        else READATRRIBUTE("nextframe:", next_frame)  
        else READATRRIBUTE("nextstate:", next_state)  
        else READATRRIBUTE("incx:", incx)
        else READATRRIBUTE("incy:", incy) 
        else READATRRIBUTE("boxx:", boxx)
        else READATRRIBUTE("boxy:", boxy)
        else READATRRIBUTE("boxw:", boxw)
        else READATRRIBUTE("boxh:", boxh) 
        else READATRRIBUTE("colboxx:", colboxx)
        else READATRRIBUTE("colboxy:", colboxy)
        else READATRRIBUTE("colboxw:", colboxw)
        else READATRRIBUTE("colboxh:", colboxh)
        if (index >=0) {
            animations->collection[index].frames[frame_index].duration = duration;
          //  animations->collection[index].frames[frame_index].flip = flip;            
            animations->collection[index].frames[frame_index].next_frame = next_frame;
            animations->collection[index].frames[frame_index].next_state = next_state;
            animations->collection[index].frames[frame_index].incx = incx;
            animations->collection[index].frames[frame_index].incy = incy;
            animations->collection[index].frames[frame_index].box.x = boxx;
            animations->collection[index].frames[frame_index].box.y = boxy;
            animations->collection[index].frames[frame_index].box.w = boxw;
            animations->collection[index].frames[frame_index].box.h = boxh;
            animations->collection[index].frames[frame_index].colbox.x = colboxx;
            animations->collection[index].frames[frame_index].colbox.y = colboxy;
            animations->collection[index].frames[frame_index].colbox.w = colboxw;
            animations->collection[index].frames[frame_index].colbox.h = colboxh;            
        }       
    }
    fclose(file);
    return true;   
}

bool HandleKeyPress(Animations* animations) {
    const bool* keys = NULL;
    keys = SDL_GetKeyboardState(NULL);
    if (keys !=NULL)
    {
        switch (animations->current)
        {
        case STATE_IDLE:
            /* code */
            if (keys[SDL_SCANCODE_LALT]) {
               animations->current = STATE_ATTACK1;
               return true;     
            }
            if (keys[SDL_SCANCODE_DOWN]) {
                animations->current = STATE_UPTODUCK;
                return true;
            } 
            if (keys[SDL_SCANCODE_RIGHT]) {

                if (!animations->facing) {
                    animations->current = STATE_RUN;
                    return true;
                }
                animations->current = STATE_TURN;
                animations->facing = false;
                return true;                
            } 
            if (keys[SDL_SCANCODE_LEFT]) {

                if (animations->facing) {
                    animations->current = STATE_RUN;                    
                    return true;
                }
                animations->current = STATE_TURN;
                animations->facing = true;
                return true;                
            } 
            if (keys[SDL_SCANCODE_SPACE]) {
                animations->current = STATE_ROLL;
                return true;
            }
            break;
        case STATE_RUN:
            if (keys[SDL_SCANCODE_SPACE]) {
                animations->current = STATE_ROLL;
                return true;
            }
            if (keys[SDL_SCANCODE_LALT]) {
               animations->current = STATE_ATTACK1;
               return true;     
            }
            if (((!animations->facing) && (!keys[SDL_SCANCODE_RIGHT])) || 
               ((animations->facing) && (!keys[SDL_SCANCODE_LEFT]))) {
                    animations->current = STATE_IDLE;
                    return true;
                }                             
            
            if (keys[SDL_SCANCODE_DOWN]) {
                animations->current = STATE_UPTODUCK;
                return true;
            } 
            break;
        case STATE_UPTODUCK:
            if (keys[SDL_SCANCODE_SPACE]) {
                animations->current = STATE_ROLL;
                return true;
            }
            
            if (!keys[SDL_SCANCODE_DOWN]) {
                animations->current = STATE_DUCKTOUP;
                return true;
                }                             
            break;
        
        default:
            break;
        }
    }
    return false;
}