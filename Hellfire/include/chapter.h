#ifndef CHAPTER_H
#define CHAPTER_H

#include "scene.h"

class Chapter
{
    public:
        void nextScene(int step);
        std::string getTitle();
        int getOrder();
        int getSceneIndex();
        int getSceneCount();
        void setSceneIndex(int index);
        bool isLastChapter();
        Scene* getActScene();
        Chapter(int odr, std::string ttl, std::vector<Scene*> scns, bool isLast);
        Chapter(std::vector<Scene*> scns);
        ~Chapter();
    protected:

    private:
        int order;
        bool last;
        std::string title;
        std::vector<Scene*> scenes;
        int actSceneIndex;
       // Chest chest;
};

#endif // CHAPTER_H
