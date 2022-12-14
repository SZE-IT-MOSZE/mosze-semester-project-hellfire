#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "enums.h"

class Weapon : public Item
{
    public:
        int getType() override;
        std::string getArt() override;
        void equip();
        void dequip();
        bool isEquipped();
        Weapon(std::string name, std::string art, std::string tp, int effectiveness) : Item(name, art, effectiveness) {
             if(tp == "sword") {
                type = WeaponType::Sword;
             }
             else {
                type = WeaponType::Staff;
             }
        };

    private:
        WeaponType type;
        bool equipped;
};


#endif // WEAPON_H
