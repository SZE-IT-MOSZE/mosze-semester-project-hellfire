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
        static void loadNextChapter(int chapterIndex);
        static void openInventory();
        static void openAttribueWindow(TTF_Font* font, TTF_Font* titleFont);
        static int openMenu(SDL_Surface* screen, TTF_Font* font, TTF_Font* titleFont, int previousState);
        static void newGame();
        static void saveGame();
        static bool loadGame();
        static int openInGameMenu(TTF_Font* font);
        static int turn(TTF_Font* font, TTF_Font* storyFont);
        static const char* GAME_TITLE;
        ~Game();
    protected:

    private:
        Game();
        static Player* player;
        static Chapter* actualChapter;
        static void openInterludeWindow(TTF_Font* titleFont, TTF_Font* font);
};

#endif // GAME_H
