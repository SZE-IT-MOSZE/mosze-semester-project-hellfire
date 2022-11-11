#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H


class Attributes
{
    public:
        int getStrength();
        int getIntelligence();
        int getPersuasion();
        void upgradeStrength();
        void upgradeIntelligence();
        void upgradePersuasion();

    protected:

    private:
        int strength;
        int intelligence;
        int persuasion;
        int corruptionLevel;
};

#endif // ATTRIBUTES_H