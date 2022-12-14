#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "item.h"
#include "enums.h"
#include "player.h"


class Consumable : public Item
{
    public:
        int getType() override;
        std::string getArt() override;
        int getCharges();
        bool isEmpty();
        void use(Player* player);
        Consumable(std::string name, std::string art, std::string tp, int ch, int effectiveness) : Item(name, art, effectiveness) {
             charges = ch;
             if(tp == "str") {
                type = ConsumableType::Strength;
             }
             else if(tp == "int") {
                type = ConsumableType::Intelligence;
             }
             else {
                type = ConsumableType::Persuasion;
             }
        };

    private:
        ConsumableType type;
        int charges;
};

#endif // CONSUMABLE_H
