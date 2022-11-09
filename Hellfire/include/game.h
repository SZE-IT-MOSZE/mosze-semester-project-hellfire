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
        static void loadGame(string name);
        static void saveHame(string name);
        static void openInventory();
        static void openAttribueWindow();
        static void openMenu();
        static void newGame();
        static void turn();

    protected:

    private:
        Game();

        static Player player;
        static Chapter actualChapter;
};

#endif // GAME_H
