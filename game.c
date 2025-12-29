/* clear.c ... */

/*
 * This example code creates an SDL window and renderer, and then clears the
 * window to a different color every frame, so you'll effectively get a window
 * that's smoothly fading between colors.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include "animation.h"
#define WINDOW_W 800
#define WINDOW_H 600

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
Animation idle;
Animation run;
Animation* CurAnimation = NULL;
SDL_Texture *sprite_texture = NULL;
Animations Hero_animation;

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
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_W, WINDOW_H, SDL_LOGICAL_PRESENTATION_STRETCH);
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
            
            keys = SDL_GetKeyboardState(NULL);
            if ((keys !=NULL) && (keys[SDL_SCANCODE_RIGHT]))
            {
                CurAnimation = &Hero_animation.collection[STATE_RUN];
            }
            break;
        case SDL_EVENT_KEY_UP:
            keys = SDL_GetKeyboardState(NULL);
            if ((keys !=NULL) && (!keys[SDL_SCANCODE_RIGHT]))
            {
                CurAnimation = &Hero_animation.collection[STATE_IDLE];
            }
            break;
    }
    
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_FRect dstrect = {100,200,SPRITE_SIZE, SPRITE_SIZE};
    
    
    
    SDL_FRect srcrect;
    srcrect = Animate(CurAnimation);

   //const int now = SDL_GetTicks() / 200;  /* convert from milliseconds to seconds. */

            
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
 //   const float green = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
  //  const float blue = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    /* put the newly-cleared rendering on the screen. */
   
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    SDL_RenderDebugText(renderer, 0, 0, "Hello world!");

    SDL_RenderTexture(renderer, sprite_texture, &srcrect, &dstrect);
     
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
    if (sprite_texture != NULL) SDL_DestroyTexture(sprite_texture);
    if (idle.frames != NULL) SDL_free(idle.frames);
    if (run.frames != NULL) SDL_free(run.frames);
    for (int i=0; i < Hero_animation.total; i++) {
        SDL_free(Hero_animation.collection[i].frames);        
    }
    SDL_free(Hero_animation.collection);
    
}