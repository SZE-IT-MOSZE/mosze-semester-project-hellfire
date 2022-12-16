#include "render.h"
#include "game.h"

const int Render::WIDTH = 1024;
const int Render::HEIGHT = 640;

//az ablak létrehozása
SDL_Window* Render::window = SDL_CreateWindow(Game::GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
                             ,Render::WIDTH, Render::HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

SDL_Renderer* Render::renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

//metódus egy adott surface kirajzolására, majd az erõforrások felszabadítására
void Render::renderSurface(SDL_Surface* surface, SDL_Rect &pos)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface);
    SDL_RenderCopy( renderer, texture,  NULL,  &pos);
    SDL_RenderPresent(renderer);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

Render::Render()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
    }
};

Render::~Render()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer );
}
