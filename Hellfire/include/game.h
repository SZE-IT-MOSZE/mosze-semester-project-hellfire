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
        static void setChapter(Chapter * newChapter);
        static Chapter loadNextChapter();
        static Player& getPlayer();
        static void loadGame(std::string name);
        static void saveHame(std::string name);
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
        static const char* STORY_PATH;
        static Player player;
        static Chapter* actualChapter;

};

#endif // GAME_H
