#ifndef CHAPTER_H
#define CHAPTER_H

#include "Chest.h"
#include "scene.h"

class Chapter
{
    public:
        bool isFinished();
        void drawScene();
        Scene* nextScene();
        std::string getTitle();
        int getOrder();
        Chapter(int odr, std::string ttl, std::vector<Scene>& scns);
        Chapter();
        Chapter& operator=(Chapter& ch);
    protected:

    private:
        bool finished;
        int order;
        std::string title;
        std::vector<Scene>& scenes;
        int actSceneIndex;
       // Chest chest;
};

#endif // CHAPTER_H
