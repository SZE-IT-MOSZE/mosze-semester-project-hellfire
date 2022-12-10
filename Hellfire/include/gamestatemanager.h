#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include "chapter.h"

class GameStateManager
{
    public:
        GameStateManager();
        Chapter* loadChapterFromXML(int chapterIndex);
};

#endif // GAMESTATEMANAGER_H

