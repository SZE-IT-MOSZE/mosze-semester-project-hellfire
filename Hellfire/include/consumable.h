#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include <item.h>


class Consumable : public Item
{
    public:
        Consumable();

    protected:

    private:
        ConsumableType type;
        int charges;
};

#endif // CONSUMABLE_H
