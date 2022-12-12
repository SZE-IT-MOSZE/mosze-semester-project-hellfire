#include "attributes.h"

using namespace std;

Attributes::Attributes()
{
    strength = 0;
    intelligence = 0;
    persuasion = 0;
    corruptionLevel = 0;
}

Attributes::Attributes(int str, int intlg, int per, int cpn)
{
    strength = str;
    intelligence = intlg;
    persuasion = per;
    corruptionLevel = cpn;
}

int Attributes::getStrength()
{
    return strength;
}

int Attributes::getIntelligence()
{
    return intelligence;
}

int Attributes::getPersuasion()
{
    return persuasion;
}

int Attributes::getCorruption() {
    return corruptionLevel;
}

void Attributes::upgradeStrength()
{
    strength += 1;
}

void Attributes::upgradeIntelligence()
{
    intelligence += 1;
}

void Attributes::upgradePersuasion()
{
    persuasion += 1;
}

void Attributes::setCorruption(int amount)
{
    corruptionLevel += amount;
}
