// Released into the public domain by Mark Schreiber <mark7@alumni.cmu.edu>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <SDL.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <stdlib.h>

static bool g_enable_fullscreen_toggle;
static bool g_enable_ungrab = true;
static bool g_enable_hdb_colorkey_fix;
static bool g_have_settings;

static void __attribute__ ((constructor)) startup_constructor() 
{
    char *str;
    
    if (!g_have_settings) {
        str = getenv("SDL_FS_TOGGLE");
        if (str && strcmp("0", str)) {
            g_enable_fullscreen_toggle = true;
        }

        str = getenv("SDL_FS_TOGGLE_NO_UNGRAB");
        if (str && strcmp("1", str)) {
            g_enable_ungrab = false;
        }

        str = getenv("SDL_HDB_COLORKEY_FIX");
        if (str && strcmp("0", str)) {
            g_enable_hdb_colorkey_fix = true;
        }
        
        g_have_settings = true;
    }
}

static void handle_key_event(SDL_Event *event)
{
    SDL_Surface *surface;
    bool becoming_fullscreen;
    bool toggle_fullscreen;

    if (!event)
        return;
    
    toggle_fullscreen = (event->type == SDL_KEYDOWN)
        && (event->key.keysym.sym == SDLK_RETURN)
        && (event->key.keysym.mod & KMOD_ALT);
    
    if (toggle_fullscreen) {
        if (g_enable_fullscreen_toggle) {
            SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
            if (g_enable_ungrab) {
                surface = SDL_GetVideoSurface();
                becoming_fullscreen = !!(surface->flags & SDL_FULLSCREEN);
                if (becoming_fullscreen) {
                    SDL_WM_GrabInput(SDL_GRAB_ON);
                } else {
                    SDL_WM_GrabInput(SDL_GRAB_OFF);
                }
            }
        }
    }
}

int SDL_WaitEvent(SDL_Event *event)
{
    int result;
    static int (*func)(SDL_Event *event);

    if (!func)
        func = dlsym(RTLD_NEXT, "SDL_WaitEvent");

    result = func(event);

    if (result) {
        handle_key_event(event);
    }
    return (result);
}

int SDL_SetColorKey(SDL_Surface *surface, Uint32 flag, Uint32 key)
{
        static int (*func)(SDL_Surface *, Uint32, Uint32);

        if (!func)
                func = dlsym(RTLD_NEXT, "SDL_SetColorKey");

        if (g_enable_hdb_colorkey_fix) {
            if (key == 0xff00ff)
                key = 0xf800f8;
        }
        return func(surface, flag, key);
}


int SDL_PollEvent(SDL_Event *event)
{
    int result;
    static int (*func)(SDL_Event *event);
    if (!func)
        func = dlsym(RTLD_NEXT, "SDL_PollEvent");

    result = func(event);

    if (result) {
        handle_key_event(event);
    }
    return (result);
}
