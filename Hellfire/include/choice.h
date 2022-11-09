#ifndef CHOICE_H
#define CHOICE_H


class Choice
{
    public:
        string getText();
        int getType();
        bool isFailed();
        bool isChosen();
        int getDifficulty();
        int getExperience();
        int getCorruption();

    protected:

    private:
        string text;
        ChoiceType type;
        bool isFailed;
        int difficulty;
        bool isChosen;
        int experience;
        int corruption;
};

#endif // CHOICE_H
