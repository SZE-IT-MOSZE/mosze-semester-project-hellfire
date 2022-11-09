#ifndef GAME_H
#define GAME_H


static class Game
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

        static player Player;
        static actualChapter Chapter;
};

#endif // GAME_H
