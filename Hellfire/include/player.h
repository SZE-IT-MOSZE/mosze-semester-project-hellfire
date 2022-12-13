#ifndef PLAYER_H
#define PLAYER_H

#include "inventory.h"
#include "attributes.h"
#include "weapon.h"

class Player
{
    public:
        Inventory* getInventory();
        Attributes* getAttributes();
        void setSkillPoints(int n);
        int getSkillPoints();
        void setExperience(int amount);
        int getExperience();
        char* determineCorruptionArt();
        Player();
        Player(int skillPoints, int experience, Attributes* attributes);
        ~Player();
    protected:

    private:
        Inventory* inventory;
        int availableSkillPoints;
        Attributes* attributes;
        std::string name;
        int experience;
       // Item& equippedItem;
};

#endif // PLAYER_H
