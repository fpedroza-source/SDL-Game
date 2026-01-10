#include "map.h"

void DrawObj(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* objects, const int index)
{
    for (int j = 0; j <MAX_ANIMATION; j++) {
        
        if (objects->collection[j].length == 0) break;
        SDL_FRect* srcrect = &objects->collection[j].frames[0].box;
        SDL_FRect dstrect = *srcrect;
        SDL_FRect colbox = objects->collection[j].frames[0].colbox;
        dstrect.x = objects->collection[j].frames[0].posxy.x - index*WINDOW_W;
        dstrect.y = objects->collection[j].frames[0].posxy.y;
        colbox.x += dstrect.x;
        colbox.y += dstrect.y;
        
        SDL_RenderTexture(renderer, tiles, srcrect, &dstrect);
        if (colbox.w > 0) {
            SDL_RenderRect(renderer, &colbox);
        }            
        
    }
}

void DrawMap(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* mapgrid, const int index)
{
    /* 1- next_state indicates how many times to repeat the tiles drawing*/
 
    Animation* cur_map = &mapgrid->collection[index];
    
    size_t length = cur_map->length;
    if (length <=0) return;
    
    for (int j = 0; j < length; j++) {
        
        int repeat = cur_map->frames[j].next_state;
        SDL_FRect* srcrect = &cur_map->frames[j].box;
        SDL_FRect dstrect = cur_map->frames[j].box;
        SDL_FRect colbox = cur_map->frames[j].colbox;
        dstrect.x = cur_map->frames[j].posxy.x;
        dstrect.y = cur_map->frames[j].posxy.y;
        
        for (int k=0; k < repeat; k++) {        
            SDL_RenderTexture(renderer, tiles, srcrect, &dstrect);
            if (colbox.w > 0) {
                SDL_RenderRect(renderer, &colbox);
                colbox.x += cur_map->frames[j].incxy.x;
                colbox.y += cur_map->frames[j].incxy.y;           
            }
            dstrect.x += cur_map->frames[j].incxy.x;
            dstrect.y += cur_map->frames[j].incxy.y;           
            
        
        }
        
    }
   
}


int CheckMapCollision(Animations* mapgrid, SDL_FRect *frame_rect, const int index)
{
    
    Animation* cur_map = &mapgrid->collection[index];

    size_t length = mapgrid->collection[index].length;
    int result = -1;
    if (length <=0) return 0;
    for (size_t j = 0; j < length; j++) {
        
        int repeat = cur_map->frames[j].next_state;
        SDL_FRect colbox = cur_map->frames[j].colbox;
        
        if (colbox.w == 0) continue;
        
        for (int k=0; k < repeat; k++)
        {
            SDL_FRect result_rect; 
         
            if (SDL_GetRectIntersectionFloat(&colbox, frame_rect, &result_rect)) {
                if (frame_rect->y + frame_rect->h != colbox.y) {
                    *frame_rect = result_rect;
                    return 1;
                }
                result = 0;
             } //else return -1;

                
            colbox.x += cur_map->frames[j].incxy.x;
            colbox.y += cur_map->frames[j].incxy.y;            
        
        }
        
    }
    return result;   
}

int CheckObjCollision(Animations* objgrid, SDL_FRect *frame_rect, const int index) {

    for (size_t j = 0; j < MAX_ANIMATION; j++) {
        
        if (objgrid->collection[j].length == 0) continue;
        
        SDL_FRect result_rect; 
        SDL_FRect colbox = objgrid->collection[j].frames[0].colbox;
        colbox.x +=objgrid->collection[j].frames[0].posxy.x - index*WINDOW_W;
        colbox.y +=objgrid->collection[j].frames[0].posxy.y; 
         
        if (SDL_GetRectIntersectionFloat(&colbox, frame_rect, &result_rect)) {
             return j;
        }                                                                       
        
    }
    return -1;        
}
