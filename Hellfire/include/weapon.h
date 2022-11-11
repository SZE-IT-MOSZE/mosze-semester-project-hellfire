#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"

enum WeaponType {
    Staff, Sword
};

class Weapon : public Item
{
    public:
        int getType();
        void equip();
        void dequip();
        bool isEquipped();

    protected:

    private:
        WeaponType type;
        bool equipped;
};


#endif // WEAPON_H
