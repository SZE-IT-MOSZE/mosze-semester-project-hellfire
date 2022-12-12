#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include "chapter.h"
#include "player.h"
#include "choice.h"
#include "tinyxml2.h"
#include <vector>

class GameStateManager
{
    public:
        GameStateManager();
        Chapter* loadChapterFromXML(int chapterIndex);
        void saveGameStateToXML(Player* player, int chapterIndex, int sceneIndex, std::vector<Choice*>* choices);
        bool loadGameStateFromXML(Player* player, Chapter* chapter);
        Player* getPlayerState();
        Chapter* getChapterState();
    private:
        void insertToXmlElement(std::string newElementName, std::string value, tinyxml2::XMLNode* parent, tinyxml2::XMLDocument* xmlDoc);
        void insertToXmlElement(std::string newElementName, int value, tinyxml2::XMLNode* parent, tinyxml2::XMLDocument* xmlDoc);
        Player* playerState;
        Chapter* chapterState;
};

#endif // GAMESTATEMANAGER_H

