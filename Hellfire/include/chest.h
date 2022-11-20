#ifndef CHEST_H
#define CHEST_H

#include "item.h"

class Chest
{
    public:
        Item& Open();
    protected:

    private:
        Item& item;
};

#endif // CHEST_H
