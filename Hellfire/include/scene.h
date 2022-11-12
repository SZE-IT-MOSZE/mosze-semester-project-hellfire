#ifndef SCENE_H
#define SCENE_H

#include "choice.h"
#include <iostream>
#include <vector>

class Scene
{
    public:
        void chooseChoice();
        Scene(std::string sb, std::string a,  std::string order, std::vector<Choice> chs);
        std::string getArt();
        std::string getStorybit();
        std::string getOrder();
        std::vector<Choice>& getChoices();
        std::vector<Choice> choices;

    protected:

    private:
        std::string art;
        std::string order;
        std::string storybit;

};

#endif // SCENE_H
