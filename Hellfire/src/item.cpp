#include "item.h"

using namespace std;

string Item::getName()
{
    return name;
}

int Item::getEffectiveness()
{
    return effectiveness;
}

Item::Item(std::string n, std::string a, int effect) {
    name = n;
    art = a;
    effectiveness = effect;
}
