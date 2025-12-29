#include "animation.h"
SDL_FRect GetSpriteRect(int col, int row, int width, int height)
{
    int x = col * width;
    int y = row * height;
    SDL_FRect dst_rect = {x, y, width, height};
    return dst_rect;
}

SDL_FRect Animate(Animation* anim)
{
    if (anim->frame_time == anim->frames[anim->frame_index].duration) {
        anim->frame_index++;
        if (anim->frame_index >= anim->length) anim->frame_index = 0;
        anim->frame_time = -1;
    }
    anim->frame_time++;
    return GetSpriteRect(anim->frames[anim->frame_index].col,
        anim->frames[anim->frame_index].row, 
        SPRITE_SIZE, SPRITE_SIZE);
};

bool LoadAnimations(Animations *animations, const char* filename)
{
    int length = 0;
    int total = 0;
    int index = -1;
    int col = 0;
    int row = 0;
    int duration = 0;
    int frame_index = 0;
    FILE *file = fopen(filename, "r");
    char text[80];
    
    
    if (file == NULL) return false;

    animations->collection = NULL;

    while (fscanf(file, "%s", text) != EOF) {
    
        if (strcmp(text, "total:") == 0) {
            fscanf(file, "%d", &total);
            animations->total = total;
            animations->collection = (Animation *)SDL_malloc(total*sizeof(Animation));
            if (animations->collection == NULL) {
                return false;
                fclose(file);
            }                                    
        }
        if (strcmp(text, "length:") == 0) {
            index++;
            fscanf(file, "%d", &length);
            animations->collection[index].frames = NULL;
            animations->collection[index].length = length;
            animations->collection[index].frame_time = 0;
            animations->collection[index].frame_index = 0;
            animations->collection[index].frames = (Frame *)SDL_malloc(length*sizeof(Frame));
            if (animations->collection[index].frames == NULL) {
                fclose(file);
                return false;
            }                        
        }
        if (strcmp(text, "frame:") == 0) {
            fscanf(file, "%d", &frame_index);            
        }
        if (strcmp(text, "col:") == 0) {
            fscanf(file, "%d", &col);            
        }
        if (strcmp(text, "row:") == 0) {
            fscanf(file, "%d", &row);            
        }
        if (strcmp(text, "duration:") == 0) {
            fscanf(file, "%d", &duration);            
        }
        if ((animations->collection != NULL) && 
            (animations->collection[index].frames != NULL)){
            animations->collection[index].frames[frame_index].col = col;
            animations->collection[index].frames[frame_index].row = row;
            animations->collection[index].frames[frame_index].duration = duration;
        }
    }

  
    fclose(file);
    return true;   
}