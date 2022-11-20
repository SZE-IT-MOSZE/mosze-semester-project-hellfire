#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "item.h"
#include "enums.h"


class Consumable : public Item
{
    public:
        int getType();
        int getCharges();
        bool isEmpty();
        void use();

    protected:

    private:
        ConsumableType type;
        int charges;
};

#endif // CONSUMABLE_H
