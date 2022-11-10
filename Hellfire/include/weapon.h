#ifndef WEAPON_H
#define WEAPON_H

#include <item.h>

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

enum WeaponType {
    Staff, Sword
};

#endif // WEAPON_H
