#include "player.h"

Player::Player(Weapon init) : equippedItem(init) {
    name = "";
    availableSkillPoints = 0;
    experience = 0;
    attributes = Attributes();
    inventory = Inventory();
}

Inventory& Player::getInventory(){
    return inventory;
}
Attributes& Player::getAttributes() {
    return attributes;
}

std::string Player::getName(){
    return name;
}

void Player::setSkillPoints(int n){
    availableSkillPoints += n;
}

int Player::getSkillPoints(){
    return availableSkillPoints;
}

void Player::setName(std::string name) {
    name = name;
}

void Player::setExperience(int amount) {
    experience += amount;
}
