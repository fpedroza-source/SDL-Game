#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <math.h>
#include <SDL3/SDL_main.h>
#include "animation.h"
#include "map.h"
Animations Hero_animation, Map, Objects;


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_Texture *sprite_texture = NULL;
static SDL_Texture *map_texture = NULL;
static SDL_Texture *obj_texture = NULL;

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  
  Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
                                             + y * surface->pitch
                                             + x * 4);
  *target_pixel = pixel;
}

Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
  
  Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
                                             + y * surface->pitch
                                             + x * 4);
  return *target_pixel;
}



bool LoadTexture(char* filename, SDL_Texture** texture)
{
    
    SDL_Surface *surface = NULL;
    
    surface = SDL_LoadPNG(filename);
    if (surface == NULL) return false;
    /*for (int x =0; x < surface->w; x++)
    {
        for (int y=0; y < surface->h;y++)
        {
            Uint32 pixel = get_pixel(surface, x, y);
            Uint32 a = pixel & 0xFF000000;
            Uint8 r, b, g;
            
            b = (pixel & 0x00FF0000) >> 16;
            g = (pixel & 0x0000FF00) >> 8;
            r = pixel & 0x000000FF;
            Uint32 c = (r + g + b)/3;

            if (c < 32) c = 0x0000;
            else if (c < 64) c = 0x0F00;
            else if (c < 128) c = 0xFF00;
            else c = 0xFFFFFF;
            
                       pixel = a | c;
            

           set_pixel(surface, x, y, pixel);  
                                    
          
    }*/
    *texture = SDL_CreateTextureFromSurface(renderer, surface);  
    SDL_DestroySurface(surface);    
    if (*texture == NULL) return false;
    return true;
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    //SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Game", 1280, 720, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    
    if (!LoadAnimations(&Hero_animation, "data/chars/hero/Hero_animations.yaml")) return SDL_APP_FAILURE;
    if (!LoadAnimations(&Map, "data/maps/Cemetery/map_tiles.yaml")) return SDL_APP_FAILURE;
    if (!LoadAnimations(&Objects, "data/maps/map_objects.yaml")) return SDL_APP_FAILURE;
   
    if (!LoadTexture(Hero_animation.filename, &sprite_texture)) return SDL_APP_FAILURE;
    if (!LoadTexture(Map.filename, &map_texture)) return SDL_APP_FAILURE;
    if (!LoadTexture(Objects.filename, &obj_texture)) return SDL_APP_FAILURE;

    Hero_animation.pos.x = 32;
    Hero_animation.pos.y = 160;
    Hero_animation.facing = false;
    Hero_animation.current = STATE_IDLE;

    SDL_SetTextureScaleMode(sprite_texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(map_texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(obj_texture, SDL_SCALEMODE_NEAREST);

    //SDL_SetTextureColorMod(sprite_texture, 0, 128, 128);
    //SDL_SetTextureColorMod(map_texture, 0, 255, 0);


    SDL_SetRenderLogicalPresentation(renderer, WINDOW_W, WINDOW_H, SDL_LOGICAL_PRESENTATION_STRETCH);
    
    SDL_SetRenderVSync(renderer, 1);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    
    switch (event->type)
    {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
        
        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_ESCAPE) return SDL_APP_SUCCESS;            
                   
    }
    
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    char debug[128];
  
   
    
    
    Animation *current = &Hero_animation.collection[Hero_animation.current];

   
    SDL_SetRenderDrawColor(renderer,0, 0, 0, SDL_ALPHA_OPAQUE);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    SDL_FRect box = current->frames[current->frame_index].box;
    
    //int flip = current->frames[current->frame_index].flip;
    int map_index = ((Hero_animation.pos.x) / WINDOW_W);
    if (map_index < 0) map_index = 0;
    DrawMap(renderer, map_texture, &Map, map_index);
    DrawObj(renderer, obj_texture, &Objects, map_index);


    SDL_FRect dstrect = {fmod(Hero_animation.pos.x, WINDOW_W)-box.w/2, 
        Hero_animation.pos.y, box.w, box.h};
   
    SDL_FRect rect = current->frames[current->frame_index].colbox;
  
    rect.x += dstrect.x;
    rect.y += dstrect.y;
    SDL_RenderRect(renderer, &rect); 

    int objcol = CheckObjCollision(&Objects, &rect, map_index);

    int mapcol = CheckMapCollision(&Map, &rect, map_index);
    switch (mapcol) {
        case 1: 
            if (Hero_animation.current == STATE_FALLING) {
                Hero_animation.current = STATE_DUCKTOUP;
                Hero_animation.pos.y -= rect.h;
            } else Hero_animation.pos.x += Hero_animation.facing ? rect.w : -rect.w;                
            break;
        case -1:
            if (Hero_animation.current != STATE_CLIMBING) {
                Hero_animation.current = STATE_FALLING;
            }
            break;
    }    
    
    Animate(&Hero_animation, objcol);

    SDL_RenderTextureRotated(renderer, sprite_texture, &box, 
    &dstrect, 0.0, NULL, Hero_animation.facing);
  
   

    sprintf(debug, "frame:%d mapindex:%d posx:%f collisio:%d sate:%d\n", current->frame_index, 
        map_index, 
        Hero_animation.pos.x-box.w/2,
        objcol, Hero_animation.current);
    
    SDL_RenderDebugText(renderer, 0, 0, debug);

    
    // Update the screen with the current render
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
    if (sprite_texture != NULL) SDL_DestroyTexture(sprite_texture);
    if (map_texture != NULL) SDL_DestroyTexture(map_texture);
    if (obj_texture != NULL) SDL_DestroyTexture(obj_texture);

} 
