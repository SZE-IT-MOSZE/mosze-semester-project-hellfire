#ifndef ITEM_H
#define ITEM_H


class Item
{
    public:
        string getName();
        int getEffectiveness();
        virtual int getType() = 0;

    protected:

    private:
        int effectiveness;
        string name;
};

#endif // ITEM_H
