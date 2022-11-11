#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"
#include <vector>
class Inventory
{
    public:
        void addItem(Item& item);
        void deleteItem(std::string name);
        std::vector<Item>& getItems();
        Item& selectedItem(std::string name);

    protected:

    private:
        std::vector<Item> items;
};

#endif // INVENTORY_H
