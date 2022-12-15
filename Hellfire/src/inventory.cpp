#include "inventory.h"
#include <algorithm>

void Inventory::addItem(Item* itemToAdd)
{
    items.push_back(itemToAdd);
}

void Inventory::deleteItem(Item* itemToDelete)
{
    std::vector<Item*>::iterator it = std::find(items.begin(), items.end(), itemToDelete);
    if(it != items.end()) {
        items.erase(it);
        delete itemToDelete;
    }
}

std::vector<Item*>& Inventory::getItems()
{
    return items;
}

int Inventory::getItemsCount() {
    return items.size();
}

Inventory::Inventory() {};

Inventory::~Inventory() {

    for (auto item : items)
    {
        delete item;
    }
    items.clear();
}
