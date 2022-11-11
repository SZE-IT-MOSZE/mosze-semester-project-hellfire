#ifndef SCENE_H
#define SCENE_H

#include "choice.h"
#include <iostream>
#include <vector>

class Scene
{
    public:
        void chooseChoice();

    protected:

    private:
        std::string art;
        std::vector<Choice> choices;
};

#endif // SCENE_H
