#ifndef ITEMREPOSITORY_H
#define ITEMREPOSITORY_H

#include "weapon.h"
#include "consumable.h"
#include "tinyxml2.h"

class ItemRepository
{
    public:
         std::vector<Weapon*> choices;
         std::vector<Consumable*> consumables;
    private:
       void loadConsumablesFromXML();
       void loadWeaponsFromXML();
};

#endif // ITEMREPOSITORY_H
