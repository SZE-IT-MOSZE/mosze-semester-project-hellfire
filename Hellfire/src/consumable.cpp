#include "consumable.h"

int Consumable::getType()
{
    return static_cast<int>(type);
}

std::string Consumable::getArt() {
    if(charges == 3) {
        return "assets/" + art + "-full.bmp";
    } else if(charges == 2) {
        return "assets/" + art + "-twothird.bmp";
    }
    else if(charges == 1) {
        return "assets/" + art + "-half.bmp";
    }
    else {
        return "assets/" + art + "-empty.bmp";
    }
}

std::string Consumable::getBaseArt() {
    return art;
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
    player->applyBuff(effectiveness, static_cast<int>(type));
    charges--;
}

