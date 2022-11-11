#ifndef CHAPTER_H
#define CHAPTER_H

#include "Chest.h"
#include "scene.h"

class Chapter
{
    public:
        bool isFinished();
        void drawScene();
        void nextScene();


    protected:

    private:
        bool finished;
        std::vector<Scene> scenes;
        int actSceneIndex;
        Chest chest;
};

#endif // CHAPTER_H
