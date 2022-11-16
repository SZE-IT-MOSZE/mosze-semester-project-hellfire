#ifndef CHAPTER_H
#define CHAPTER_H

#include "Chest.h"
#include "scene.h"

class Chapter
{
    public:
        bool isFinished();
        void drawScene();
        void nextScene(int step);
        std::string getTitle();
        int getOrder();
        int getSceneIndex();
        int getSceneCount();
        Scene* getActScene();
        Chapter(int odr, std::string ttl, std::vector<Scene*> scns);
        Chapter(std::vector<Scene*> scns);
        ~Chapter();
    protected:

    private:
        bool finished;
        int order;
        std::string title;
        std::vector<Scene*> scenes;
        int actSceneIndex;
       // Chest chest;
};

#endif // CHAPTER_H
