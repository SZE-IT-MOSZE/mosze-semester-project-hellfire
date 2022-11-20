#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Render
{
    public:
        static void renderSurface(SDL_Surface* surface, SDL_Rect &pos);
        static SDL_Renderer* renderer;
        static SDL_Window *window;
        static const int WIDTH, HEIGHT;
    protected:

    private:
        Render();
        ~Render();
};

#endif
