#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include "animation.h"
Animations Hero_animation;
#define WINDOW_W 960
#define WINDOW_H 540

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
Animation* CurAnimation = NULL;
int current_state = STATE_IDLE;
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
    sprite_sheet = SDL_LoadBMP("data/hero.bmp");
    if (sprite_sheet == NULL) return SDL_APP_FAILURE;
    SDL_SetSurfaceColorKey(sprite_sheet, true, 0xFFFFFF);
    sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_sheet);  
    SDL_DestroySurface(sprite_sheet);
    if (sprite_texture == NULL) return SDL_APP_FAILURE;

    if (!LoadAnimations(&Hero_animation, "data/Hero_animations.yaml")) return SDL_APP_FAILURE;
    CurAnimation = &Hero_animation.collection[STATE_IDLE];
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
        case SDL_EVENT_KEY_UP:
            if (HandleKeyPress(CurAnimation, &current_state)) {
                CurAnimation = &Hero_animation.collection[current_state];
                CurAnimation->frame_index = 0;
                CurAnimation->frame_time = 0;
            }
                       
            break;
                   
    }
    
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_FRect dstrect = {0,0,SPRITE_SIZE, SPRITE_SIZE};
    
    char debug[80];
    
    if (Animate(CurAnimation, &current_state))
    {
        CurAnimation = &Hero_animation.collection[current_state];
        CurAnimation->frame_index = 0;
        CurAnimation->frame_time = 0;
    };

    SDL_FRect srcrect = GetSpriteRect(CurAnimation);

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    sprintf(debug, "frame:%d", CurAnimation->frame_index);
    
    SDL_RenderDebugText(renderer, 0, 0, debug);
  
    int flip = CurAnimation->frames[CurAnimation->frame_index].flip;

    SDL_RenderTextureRotated(renderer, sprite_texture, &srcrect, 
    &dstrect, 0, NULL, flip);

   
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
    if (sprite_texture != NULL) SDL_DestroyTexture(sprite_texture);
    if (Hero_animation.collection !=NULL) {
        for (int i=0; i < Hero_animation.total; i++) {
            Frame *frames = Hero_animation.collection[i].frames;        
            if (frames != NULL) SDL_free(frames);        
        }
        SDL_free(Hero_animation.collection);
    }
    
}