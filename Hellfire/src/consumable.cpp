#include "consumable.h"

int Consumable::getType()
{
    return static_cast<int>(type);
}

//a consumable t�lt�tts�gi szintje alapj�n m�s art ker�l visszaad�sra
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

//baseArt a xml-es ment�s egyszer�s�t�s�re
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

//ha az adott consumable �res akkor visszat�r�nk nem kap a player buffot
void Consumable::use(Player* player)
{
    if(isEmpty())
        return;
    player->applyBuff(effectiveness, static_cast<int>(type));
    charges--;
}

