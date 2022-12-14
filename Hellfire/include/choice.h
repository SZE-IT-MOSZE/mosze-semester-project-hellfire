#ifndef CHOICE_H
#define CHOICE_H
#include <iostream>
#include "enums.h"

class Choice
{
    public:
        std::string getText();
        int getType();
        bool isFailed();
        bool isChosen();
        int getDifficulty();
        int getExperience();
        int getCorruption();
        int getStep();
        void setFailed();
        Choice(int dif, int cor, std::string txt, int exp, std::string tp, int stp);

    protected:

    private:
        std::string text;
        ChoiceType type;
        bool failed;
        int difficulty;
        bool chosen;
        int experience;
        int corruption;
        int step;
};

#endif // CHOICE_H
