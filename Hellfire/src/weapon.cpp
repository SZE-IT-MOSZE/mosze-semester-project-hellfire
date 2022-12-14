#include "weapon.h"

int Weapon::getType()
{
    return static_cast<int>(type);
}

std::string Weapon::getArt() {
    return art;
}

void Weapon::equip()
{
    if(equipped)
        return;
    equipped = true;
}

void Weapon::dequip()
{
    if(equipped)
        equipped = false;
}

bool Weapon::isEquipped()
{
    return equipped;
}
