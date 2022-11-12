#ifndef GAME_H
#define GAME_H

#include "Chapter.h"
#include "Player.h"

class Game
{
    public:
        static Chapter& getChapter();
        static Chapter loadNextChapter();
        static Player& getPlayer();
        static void loadGame(std::string name);
        static void saveHame(std::string name);
        static void openInventory();
        static void openAttribueWindow();
        static void openMenu();
        static void newGame();
        static void turn();
    protected:

    private:
        Game();
        static const char* STORY_PATH;
        static Player player;
        static Chapter actualChapter;

};

#endif // GAME_H
