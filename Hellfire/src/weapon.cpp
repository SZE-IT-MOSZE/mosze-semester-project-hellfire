#include "weapon.h"

int Weapon::getType()
{
    return static_cast<int>(type);
}

void Weapon::equip()
{

}

void Weapon::dequip()
{

}

bool Weapon::isEquipped()
{
    return equipped;
}
