#ifndef GAME_H
#define GAME_H

#include "Chapter.h"
#include "Player.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Game
{
    public:
        static Chapter* getChapter();
        static Player* getPlayer();
        static void setChapter(Chapter * newChapter);
        static void loadNextChapter();
        static void openInventory();
        static void openAttribueWindow();
        static int openMenu(SDL_Surface* screen, TTF_Font* font, TTF_Font* titleFont, int previousState);
        static void newGame();
        static int turn(TTF_Font* font, TTF_Font* storyFont);
        static const char* GAME_TITLE;
        ~Game();
    protected:

    private:
        Game();
        static Player* player;
        static Chapter* actualChapter;

};

#endif // GAME_H
