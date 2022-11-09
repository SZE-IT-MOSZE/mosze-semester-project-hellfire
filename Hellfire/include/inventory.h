#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"

class Inventory
{
    public:
        void addItem(Item item);
        void deleteItem(string name);
        vector<Item>& getItems();
        Item& selectedItem(string name);

    protected:

    private:
        vector<Item> items;
};

#endif // INVENTORY_H
