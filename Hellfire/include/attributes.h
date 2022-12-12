#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

class Attributes
{
    public:
        int getStrength();
        int getIntelligence();
        int getPersuasion();
        int getCorruption();
        void upgradeStrength();
        void upgradeIntelligence();
        void upgradePersuasion();
        void setCorruption(int amount);
        Attributes();
        Attributes(int str, int intlg, int per, int cpn);

    private:
        int strength;
        int intelligence;
        int persuasion;
        int corruptionLevel;
};

#endif // ATTRIBUTES_H
