#include "map.h"
void DrawBackGround(SDL_Renderer *renderer, SDL_Texture *tiles, Animations* mapgrid, int index)
{
    /* 1 - the first animation (collercion[0]) contains the tiles sprites information
       2- next_frame now is the tile index
       2- next_state is how many times to repeat the tiles drawing*/

    Animation* sprites = &mapgrid->collection[0];
    
    size_t length = mapgrid->collection[index].length;
    if ((index <= 0) || (length <=0)) return;
    for (size_t j = 0; j < length; j++) {
        int tile = mapgrid->collection[index].frames[j].next_frame;
        //Bound tile to the max number of tiles
        if (tile > sprites->length - 1) {
            tile = sprites->length - 1;
        }
        int repeat = mapgrid->collection[index].frames[j].next_state;
        SDL_FRect* srcrect = &sprites->frames[tile].box;
        SDL_FRect dstrect = mapgrid->collection[index].frames[j].box;
        dstrect.w = srcrect->w;
        dstrect.h = srcrect->h;          
        
        for (size_t k=0; k < repeat; k++)
        {
            SDL_FRect rect = sprites->frames[tile].colbox;
            rect.x += dstrect.x;
            rect.y += dstrect.y;
            
            SDL_RenderTexture(renderer, tiles, srcrect, &dstrect);
            SDL_RenderRect(renderer, &rect);
            dstrect.x += mapgrid->collection[index].frames[j].incx;
            dstrect.y += mapgrid->collection[index].frames[j].incy;            
        
        }
        
    }
    

}
