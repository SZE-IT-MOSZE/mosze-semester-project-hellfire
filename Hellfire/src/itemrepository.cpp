#include "itemrepository.h"

using namespace tinyxml2;

ItemRepository::ItemRepository() {};

Consumable* ItemRepository::loadRandomConsumableFromXML() {
    XMLDocument xmlConsumables;
    XMLError result = xmlConsumables.LoadFile("assets/potions.xml");
    if(result != XML_SUCCESS) {
        return nullptr;
    }
    XMLElement * root = xmlConsumables.RootElement();
    int nOfPotions = atoi(root -> Attribute("nOfPotions"));
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, nOfPotions - 1);
    int random_integer = uni(rng);

    XMLElement* potion = root->FirstChildElement();
    for (int i = 0; potion != NULL && i < random_integer; i++) {
        potion = potion->NextSiblingElement();
    }

    if(potion != NULL) {
        XMLElement * pName = potion -> FirstChildElement("name");
        XMLElement * pArt = potion -> FirstChildElement("art");
        XMLElement * pType = potion -> FirstChildElement("type");
        XMLElement * pCharges = potion -> FirstChildElement("charges");
        XMLElement * pEffect = potion -> FirstChildElement("effect");

        return new Consumable(pName->GetText(), pArt->GetText(),
                                                pType->GetText(),
                                                atoi(pCharges->GetText()),
                                                atoi(pEffect->GetText())
                                                );
    }

   return nullptr;
}

Weapon* ItemRepository::loadRandomWeaponFromXML() {
    XMLDocument xmlWeapons;
    XMLError result = xmlWeapons.LoadFile("assets/weapons.xml");
    if(result != XML_SUCCESS) {
        return nullptr;
    }
    XMLElement * root = xmlWeapons.RootElement();
    int nOfWeapons = atoi(root -> Attribute("nOfWeapons"));
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, nOfWeapons - 1);
    int random_integer = uni(rng);

    XMLElement* weapon = root->FirstChildElement();
    for (int i = 0; weapon != NULL && i < random_integer; i++) {
       weapon = weapon->NextSiblingElement();
    }

    if(weapon != NULL) {
        XMLElement * pName = weapon -> FirstChildElement("name");
        XMLElement * pArt = weapon -> FirstChildElement("art");
        XMLElement * pType = weapon -> FirstChildElement("type");
        XMLElement * pEffect = weapon -> FirstChildElement("effect");

        return new Weapon(pName->GetText(), pArt->GetText(),
                                                pType->GetText(),
                                                atoi(pEffect->GetText())
                                                );
    }
    return nullptr;
}

Item* ItemRepository::getRandomItem() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, 20);
    int random_integer = uni(rng);
    if(random_integer == 20) {
        return loadRandomWeaponFromXML();
    }
    return loadRandomConsumableFromXML();
}
