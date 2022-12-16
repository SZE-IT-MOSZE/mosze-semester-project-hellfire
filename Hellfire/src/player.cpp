#include "player.h"

Player::Player()
{
    availableSkillPoints = 5;
    experience = 0;
    buff = 0;
    buffType = -1;
    attributes = new Attributes();
    inventory = new Inventory();
    equippedWeapon = nullptr;
}

Player::Player(int skillPoints, int exp, int bf, int bfTp, Attributes* attrb, Inventory* inv, Weapon* wpn) {
    availableSkillPoints = skillPoints;
    experience = exp;
    buff = bf;
    buffType = bfTp;

    attributes = attrb;
    inventory = inv;
    equippedWeapon = wpn;
    if(wpn != nullptr) {
        wpn->equip();
    }
}

int Player::getExperience()
{
    return experience;
}

void Player::applyBuff(int effectiveness, int type) {
    buff = effectiveness;
    buffType = type;
}

void Player::removeBuff() {
    buff = 0;
    buffType = -1;
}

bool Player::hasBuff() {
    return buff != 0;
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

//corruption alapján a megfelelõ art visszaküldése
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

Weapon* Player::getEquippedWeapon() {
    return equippedWeapon;
}

void Player::setEquippedWeapon(Weapon* weapon) {
    if(weapon == nullptr) {
        equippedWeapon = nullptr;
        return;
    }
    if(weapon->isEquipped()) {
        weapon->dequip();
        equippedWeapon = nullptr;
        return;
    }
    if(equippedWeapon != nullptr) {
        equippedWeapon->dequip();
        equippedWeapon = nullptr;
    }
    equippedWeapon = weapon;
    equippedWeapon->equip();
}

int Player::getBuff() {
    return buff;
}

int Player::getBuffType() {
    return buffType;
}

Player::~Player()
{
    delete attributes;
    delete inventory;
}
