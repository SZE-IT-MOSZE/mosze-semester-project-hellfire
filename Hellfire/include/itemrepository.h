#ifndef ITEMREPOSITORY_H
#define ITEMREPOSITORY_H

#include "weapon.h"
#include "item.h"
#include "consumable.h"
#include "tinyxml2.h"
#include <random>

class ItemRepository
{
    public:
        Item* getRandomItem();
        ItemRepository();
    private:
       Consumable* loadRandomConsumableFromXML();
       Weapon* loadRandomWeaponFromXML();
};

#endif // ITEMREPOSITORY_H
