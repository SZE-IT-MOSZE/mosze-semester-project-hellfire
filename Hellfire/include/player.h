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
        Weapon* getEquippedWeapon();
        void setEquippedWeapon(Weapon* wpn);
        int getExperience();
        void applyBuff(int effectiveness, int type);
        void removeBuff();
        bool hasBuff();
        int getBuffType();
        int getBuff();
        std::string determineCorruptionArt();
        Player();
        Player(int skillPoints, int exp, int bf, int bfTp, Attributes* attrb, Inventory* inv, Weapon* wpn);
        ~Player();
    protected:

    private:
        Inventory* inventory;
        int availableSkillPoints;
        Attributes* attributes;
        int experience;
        int buff;
        int buffType;
        Weapon* equippedWeapon;
};

#endif // PLAYER_H
