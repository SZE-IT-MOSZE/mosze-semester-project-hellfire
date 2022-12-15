#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"
#include <vector>
class Inventory
{
    public:
        void addItem(Item* itemToAdd);
        void deleteItem(Item* itemToDelete);
        std::vector<Item*>& getItems();
        int getItemsCount();
        Inventory();
        ~Inventory();
    private:
        std::vector<Item*> items;
};

#endif // INVENTORY_H
