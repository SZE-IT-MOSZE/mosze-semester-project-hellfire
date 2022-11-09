#ifndef SCENE_H
#define SCENE_H

#include "choice.h"

class scene
{
    public:
        void chooseChoice();

    protected:

    private:
        string art;
        vector<Choice> choices;
};

#endif // SCENE_H
