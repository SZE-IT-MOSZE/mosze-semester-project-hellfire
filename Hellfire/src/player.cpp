#include "player.h"

Player::Player()
{
    availableSkillPoints = 5;
    experience = 0;
    attributes = new Attributes();
    inventory = new Inventory();
}

Player::Player(int skillPoints, int exp, Attributes* attrb) {
    availableSkillPoints = skillPoints;
    experience = exp;
    attributes = attrb;
    inventory = new Inventory();
}

int Player::getExperience()
{
    return experience;
}

Inventory* Player::getInventory()
{
    return inventory;
}
Attributes* Player::getAttributes()
{
    return attributes;
}

void Player::setSkillPoints(int n)
{
    availableSkillPoints += n;
}

int Player::getSkillPoints()
{
    return availableSkillPoints;
}

void Player::setExperience(int amount)
{
    experience += amount;
}

std::string Player::determineCorruptionArt() {
    int corruption = attributes->getCorruption();
    if(corruption >= -1  && corruption <= 1) {
        return "assets/Necklace_half.bmp";
    }
    else if(corruption == 2 || corruption  == 3) {
        return "assets/Necklace_twothird.bmp";
    }
    else if(corruption > 3) {
        return "assets/Necklace_black2.bmp";
    }
    else if(corruption == -2 || corruption == -3) {
         return "assets/Necklace_onethird.bmp";
    }
    else {
         return "assets/Necklace.bmp";
    }
}

Player::~Player()
{
    delete attributes;
    delete inventory;
}
