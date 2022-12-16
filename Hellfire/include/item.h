#ifndef ITEM_H
#define ITEM_H
#include <iostream>

class Item
{
    public:
        virtual std::string getName();
        virtual std::string getArt() = 0;
        int getEffectiveness();
        virtual int getType() = 0;
        Item(std::string name, std::string art, int effectiveness);
    protected:
        int effectiveness;
        std::string name;
        std::string art;
};

#endif // ITEM_H
