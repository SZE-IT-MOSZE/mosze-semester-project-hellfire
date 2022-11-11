#ifndef ITEM_H
#define ITEM_H
#include <iostream>

class Item
{
    public:
        std::string getName();
        int getEffectiveness();
        virtual int getType() = 0;

    protected:

    private:
        int effectiveness;
        std::string name;
};

#endif // ITEM_H
