#ifndef ITEM_H
#define ITEM_H
#include <iostream>

class Item
{
    public:
        std::string getName();
        virtual std::string getArt() = 0;
        int getEffectiveness();
        virtual int getType() = 0;

    protected:

    private:
        int effectiveness;
        std::string name;
        std::string art;
};

#endif // ITEM_H
