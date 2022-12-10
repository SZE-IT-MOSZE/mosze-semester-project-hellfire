#ifndef SCENE_H
#define SCENE_H

#include "choice.h"
#include <iostream>
#include <vector>
#include "player.h"

class Scene
{
    public:
        void chooseChoice(Choice* chosenChoice, Player* player);
        Scene(std::string sb, std::string a,  std::string order, std::vector<Choice*> chs);
        std::string getArt();
        std::string getStorybit();
        std::string getOrder();
        std::vector<Choice*>& getChoices();
        std::vector<Choice*> choices;
        ~Scene();
    protected:

    private:
        std::string art;
        std::string order;
        std::string storybit;
};

#endif // SCENE_H
