#include "consumable.h"

int Consumable::getType()
{
    return static_cast<int>(type);
}

//a consumable töltöttségi szintje alapján más art kerül visszaadásra
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

//baseArt a xml-es mentés egyszerûsítésére
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

//ha az adott consumable üres akkor visszatérünk nem kap a player buffot
void Consumable::use(Player* player)
{
    if(isEmpty())
        return;
    player->applyBuff(effectiveness, static_cast<int>(type));
    charges--;
}

