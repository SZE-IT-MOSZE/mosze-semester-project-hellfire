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

Player::~Player()
{
    delete attributes;
    delete inventory;
}
