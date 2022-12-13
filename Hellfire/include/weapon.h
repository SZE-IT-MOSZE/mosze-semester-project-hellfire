#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "enums.h"

class Weapon : public Item
{
    public:
        int getType();
        void equip();
        void dequip();
        bool isEquipped();
        Weapon(std::string artText, std::string typeText);

    protected:

    private:
        WeaponType type;
        std::string art;
        bool equipped;
};


#endif // WEAPON_H
