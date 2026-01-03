#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include "animation.h"
Animations Hero_animation;
#define WINDOW_W 960
#define WINDOW_H 540

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_Texture *sprite_texture = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface *sprite_sheet = NULL;

    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", WINDOW_W, WINDOW_H, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_W, WINDOW_H, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync(renderer, 1);
    sprite_sheet = SDL_LoadPNG("data/hero.png");
    if (sprite_sheet == NULL) return SDL_APP_FAILURE;
    //SDL_SetSurfaceColorKey(sprite_sheet, true, 0xFFFFFF);
    sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_sheet);  
    SDL_DestroySurface(sprite_sheet);
    if (sprite_texture == NULL) return SDL_APP_FAILURE;

    if (!LoadAnimations(&Hero_animation, "data/Hero_animations.yaml")) return SDL_APP_FAILURE;
    Hero_animation.pos.x = 0;
    Hero_animation.pos.y = 128;
    Hero_animation.facing = false;
    Hero_animation.current = STATE_IDLE;
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    const bool* keys = NULL;
    
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
    char debug[80];
  
    Animate(&Hero_animation);
    
    
    Animation *current = &Hero_animation.collection[Hero_animation.current];

    SDL_FRect srcrect = GetSpriteRect(current);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    SDL_FRect box = current->frames[current->frame_index].box;
    box.x += Hero_animation.pos.x;
    box.y += Hero_animation.pos.y;
    /*if (!Hero_animation.facing)
    {
        box.x -=/
    }*/
    SDL_RenderRect(renderer, &box);

    sprintf(debug, "frame:%d", current->frame_index);
    
    SDL_RenderDebugText(renderer, 0, 0, debug);
  
    //int flip = current->frames[current->frame_index].flip;

    SDL_FRect dstrect = {Hero_animation.pos.x - SPRITE_SIZE_W/2, Hero_animation.pos.y,SPRITE_SIZE_W, SPRITE_SIZE_H};
   
    SDL_RenderTextureRotated(renderer, sprite_texture, &srcrect, 
    &dstrect, 0.0, &Hero_animation.pos, Hero_animation.facing);

   
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
    if (sprite_texture != NULL) SDL_DestroyTexture(sprite_texture);

} 
