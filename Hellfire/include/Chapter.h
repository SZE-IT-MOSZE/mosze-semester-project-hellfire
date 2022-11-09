#ifndef CHAPTER_H
#define CHAPTER_H

#include "Chest.h"
#include "Scene.h"

class Chapter
{
    public:
        bool isFinished();
        void drawScene();
        void nextScene();


    protected:

    private:
        bool isFinished;
        vector<Scene> scenes
        int actSceneIndex;
        Chest chest;
};

#endif // CHAPTER_H
