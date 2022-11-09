#ifndef PLAYER_H
#define PLAYER_H

#include "inventory.h"
#include "attributes.h"

class Player
{
    public:
        Inventory& getInventory();
        Attributes& getAttributes();
        string getName();
        void setSkillPoints(int n);
        int getSkillPoints();
        void setName(string name);
        void setExperience(int amount);

    protected:

    private:
        Inventory inventory;
        int availableSkillPoints;
        Attributes attributes;
        string name;
        int experience;
        Item equippedItem;
};

#endif // PLAYER_H
