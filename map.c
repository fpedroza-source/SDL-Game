#include "map.h"

void DrawObj(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* objects, const float map_pos)
{
    for (int j = 0; j <MAX_ANIMATION; j++) {
        
        if (objects->collection[j].length == 0) continue;
        SDL_FRect* srcrect = &objects->collection[j].frames[0].box;
        SDL_FRect dstrect = *srcrect;
        SDL_FRect colbox = objects->collection[j].frames[0].colbox;
        dstrect.x = objects->collection[j].frames[0].posxy.x - map_pos;
        dstrect.y = objects->collection[j].frames[0].posxy.y;
        colbox.x -= map_pos;
                
        SDL_RenderTexture(renderer, tiles, srcrect, &dstrect);
        if (colbox.w > 0) {
            SDL_RenderRect(renderer, &colbox);
        }            
        
    }
}

void DrawMap(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* mapgrid, const float map_pos)
{

    for (int i = 0; i < MAX_ANIMATION; i++)
    {

        Animation* cur_map = &mapgrid->collection[i];
        size_t length = cur_map->length;
        if (length <=0) continue;
    
        for (int j = 0; j < length; j++) {
            
            /* 1- next_state indicates how many times to repeat the tiles drawing*/      
            int repeat = cur_map->frames[j].next_state;
            SDL_FRect* srcrect = &cur_map->frames[j].box;
            SDL_FRect dstrect = cur_map->frames[j].box;
            SDL_FRect colbox = cur_map->frames[j].colbox;
            dstrect.x = cur_map->frames[j].posxy.x - map_pos;
            
            dstrect.y = cur_map->frames[j].posxy.y;
            colbox.x -= map_pos;

            if (colbox.w > 0) {
                SDL_RenderRect(renderer, &colbox);
            }

            for (int k=0; k < repeat; k++) {        
                SDL_RenderTexture(renderer, tiles, srcrect, &dstrect);                
                dstrect.x += cur_map->frames[j].incxy.x;
                dstrect.y += cur_map->frames[j].incxy.y;                               
            }
        }
        
    }
   
}


int CheckMapCollision(Animations* mapgrid, Animations* hero, SDL_FRect* result_rect)
{
    
    Animation* current = &hero->collection[hero->current];
    SDL_FRect charbox = current->frames[current->frame_index].colbox;
    int result = -1;

    charbox.x += hero->pos.x;
    charbox.y += hero->pos.y;

    
    for (int i = 0; i < MAX_ANIMATION; i++)
    {

        Animation* cur_map = &mapgrid->collection[i];
        size_t length = cur_map->length;
        if (length <=0) continue;
    
    
        for (size_t j = 0; j < length; j++) {
        
            SDL_FRect colbox = cur_map->frames[j].colbox;
            if (colbox.w == 0) continue;                    
         
            if (SDL_GetRectIntersectionFloat(&colbox, &charbox, result_rect)) {
                if (charbox.y + charbox.h != colbox.y) {
                    return 1;                    
                } //else return -1;
                result = 0;      
            }         
        
        }
        
    }
    return result;   
}

enum Objects CheckObjCollision(Animations* objgrid, Animations* hero, SDL_FRect* result_rect) {

    Animation* current = &hero->collection[hero->current];
    SDL_FRect charbox = current->frames[current->frame_index].colbox;
    
    charbox.x += hero->pos.x;
    charbox.y += hero->pos.y;


    for (size_t j = 0; j < MAX_ANIMATION; j++) {
        
        if (objgrid->collection[j].length == 0) continue;
        
        SDL_FRect objbox = objgrid->collection[j].frames[0].colbox;
                 
        if (SDL_GetRectIntersectionFloat(&charbox, &objbox, result_rect)) {             
             return j;
        }                                                                       
        
    }
    return OBJ_NONE;        
}
