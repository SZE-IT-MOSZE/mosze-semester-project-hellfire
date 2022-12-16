#include "weapon.h"

int Weapon::getType()
{
    return static_cast<int>(type);
}

std::string Weapon::getName() {
    if(equipped) {
        return name + " [Equipped]";
    }
    return name;
}

std::string Weapon::getBaseName() {
    return name;
}

std::string Weapon::getArt() {
    return "assets/" + art;
}

//base art a xml-es mentés megsegítésére
std::string Weapon::getBaseArt() {
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

