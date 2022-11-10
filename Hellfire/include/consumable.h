#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include <item.h>

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

enum ConsumableType{
    Strength,
    Intelligence,
    Persuasion
};

#endif // CONSUMABLE_H
