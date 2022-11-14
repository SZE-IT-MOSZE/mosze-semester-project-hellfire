#ifndef PLAYER_H
#define PLAYER_H

#include "inventory.h"
#include "attributes.h"
#include "weapon.h"

class Player
{
    public:
        Inventory& getInventory();
        Attributes& getAttributes();
        std::string getName();
        void setSkillPoints(int n);
        int getSkillPoints();
        void setName(std::string name);
        void setExperience(int amount);
        Player(Weapon init);
    protected:

    private:
        Inventory inventory;
        int availableSkillPoints;
        Attributes attributes;
        std::string name;
        int experience;
        Item& equippedItem;
};

#endif // PLAYER_H
