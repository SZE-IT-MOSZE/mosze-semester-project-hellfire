#include "attributes.h"

using namespace std;

Attributes::Attributes() {
    strength = 0;
    intelligence = 0;
    persuasion = 0;
    corruptionLevel = 0;
}

int Attributes::getStrength() {
    return strength;
}

int Attributes::getIntelligence(){
    return intelligence;
}

int Attributes::getPersuasion(){
    return persuasion;
}

void Attributes::upgradeStrength(){
     strength += 1;
}

void Attributes::upgradeIntelligence(){
    intelligence += 1;
}

void Attributes::upgradePersuasion() {
    persuasion += 1;
}

void Attributes::setCorruption(int amount) {
    corruptionLevel += amount;
}
