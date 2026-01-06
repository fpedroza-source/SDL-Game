#include "map.h"


void DrawMap(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* mapgrid, int index)
{
    /* 1 - the first animation (collercion[0]) contains the tiles sprites information
       2- next_frame now is the tile index
       2- next_state is how many times to repeat the tiles drawing*/
 //  index = 1;
    Animation* sprites = &mapgrid->collection[MAPTILES];
    Animation* cur_map = &mapgrid->collection[index + MAPCOORDS];
    
    size_t length = cur_map->length;
    if (length <=0) return;
    
    for (int j = 0; j < length; j++) {
        
        int tile = cur_map->frames[j].next_frame;
        //Bound tile to the max number of tiles
        if (tile >= sprites->length) {
            tile = sprites->length - 1;
        }
        int repeat = cur_map->frames[j].next_state;
        SDL_FRect* srcrect = &sprites->frames[tile].box;
        SDL_FRect dstrect = cur_map->frames[j].box;
        SDL_FRect rect = cur_map->frames[j].colbox;
       
        dstrect.w = srcrect->w;
        dstrect.h = srcrect->h;
        
        for (int k=0; k < repeat; k++) {        
            SDL_RenderTexture(renderer, tiles, srcrect, &dstrect);
            if (rect.w > 0) SDL_RenderRect(renderer, &rect);
            dstrect.x += cur_map->frames[j].incx;
            dstrect.y += cur_map->frames[j].incy;           
            rect.x += cur_map->frames[j].incx;
            rect.y += cur_map->frames[j].incy;           
        
        }
        
    }
   
}


int CheckMapCollision(SDL_Renderer *renderer, Animations* mapgrid, SDL_FRect *frame_rect, int index)
{
    
    /*Animation* boundaries = &mapgrid->collection[index];
    
    size_t length = boundaries->length;
    if (length <=0) return false;

    for (int j = 0; j < length; j++) {
        
    
        SDL_RenderRect(renderer, &boundaries->frames[j].colbox);
    }*/
    Animation* cur_map = &mapgrid->collection[index + MAPCOORDS];

    size_t length = mapgrid->collection[index].length;
    int result = -1;
    if (length <=0) return false;
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

                
            colbox.x += cur_map->frames[j].incx;
            colbox.y += cur_map->frames[j].incy;            
        
        }
        
    }
    return result;
   
}
