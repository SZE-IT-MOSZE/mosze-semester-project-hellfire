#include <iostream>
#include "game.h"
#include "render.h"

int main(int argc, char *argv[])
{
    SDL_Surface* windowSurface = SDL_GetWindowSurface(Render::window);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("assets/arial.ttf", 30);
    TTF_Font *storyFont = TTF_OpenFont("assets/arial.ttf", 20);
    TTF_Font *titleFont = TTF_OpenFont("assets/hobbiton.ttf", 50);

    int result = 0;
    while(true)
    {
        //A fõmenü megnyitásával kezd a program, és a visszatérési érték alapján, új játék, betöltés, bezárás hajtódik végre
        result = Game::openMenu(windowSurface, font, titleFont, result);
        if(result == MenuOptions::EXIT_GAME)
        {
            break;
        }
        else if(result == MenuOptions::NEW_GAME)
        {
            Game::newGame();

            while(true)
            {
                if(Game::turn(font, storyFont) == -10)
                {
                    break;
                }
            }
        }
        else if(result == MenuOptions::LOAD_GAME) {
            if(Game::loadGame()) {
                while(true)
                {
                    if(Game::turn(font, storyFont) == -10)
                    {
                        break;
                    }
                }
            }
        }
    }

    SDL_FreeSurface(windowSurface);
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_CloseFont(titleFont);
    TTF_CloseFont(storyFont);
    TTF_Quit();
    if(Game::getPlayer() != nullptr)
        delete Game::getPlayer();
    if(Game::getChapter() != nullptr)
        delete Game::getChapter();
    return EXIT_SUCCESS;
}
