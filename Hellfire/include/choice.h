#ifndef CHOICE_H
#define CHOICE_H
#include <iostream>

enum ChoiceType {
    Strength,
    Intelligence,
    Persuasion,
    Default
};

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

    protected:

    private:
        std::string text;
        ChoiceType type;
        bool failed;
        int difficulty;
        bool chosen;
        int experience;
        int corruption;
};

#endif // CHOICE_H
