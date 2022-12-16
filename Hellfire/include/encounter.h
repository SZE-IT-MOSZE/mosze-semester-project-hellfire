#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include <iostream>
#include "player.h"

class Encounter
{
    public:
        Encounter(std::string sb, std::string opt, int tp);
        int getType();
        std::string getStorybit();
        std::string getOption();
        void attributeCheck(int difficulty, Player* player);
        int d20numberGenerator();
        void engage(int difficulty, Player* player);
    private:
        std::string storybit;
        std::string option;
        int type;
};

#endif // ENCOUNTER_H
