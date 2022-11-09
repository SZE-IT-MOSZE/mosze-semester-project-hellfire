#ifndef SCENE_H
#define SCENE_H

#include "choice.h"

class Scene
{
    public:
        void chooseChoice();

    protected:

    private:
        string art;
        vector<Choice> choices;
};

#endif // SCENE_H
