#include "consumable.h"

int Consumable::getType()
{
    return static_cast<int>(type);
}

std::string Consumable::getArt() {
    if(charges == 3) {
        return art + "-full.bmp";
    } else if(charges == 2) {
        return art + "-twothird.bmp";
    }
    else if(charges == 1) {
        return art + "-half.bmp";
    }
    else {
        return art + "-empty.bmp";
    }
}

int Consumable::getCharges()
{
    return charges;
}

bool Consumable::isEmpty()
{
    return charges == 0;
}

void Consumable::use(Player* player)
{
    if(isEmpty())
        return;
}
