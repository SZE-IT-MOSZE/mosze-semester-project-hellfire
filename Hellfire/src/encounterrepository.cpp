#include "encounterrepository.h"

using namespace tinyxml2;

EncounterRepository::EncounterRepository() {};

//Egy encounter betöltése random generált index alapján az encounters.xml-bõl
Encounter* EncounterRepository::loadRandomEncounterFromXML() {
    XMLDocument xmlEncounters;
    XMLError result = xmlEncounters.LoadFile("assets/encounters.xml");
    if(result != XML_SUCCESS) {
        return nullptr;
    }
    XMLElement * root = xmlEncounters.RootElement();
    int nOfEncounters = atoi(root -> Attribute("nOfEncounters"));
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, nOfEncounters - 1);
    int random_integer = uni(rng);

    XMLElement* encounter = root->FirstChildElement();
    for (int i = 0; encounter != NULL && i < random_integer; i++) {
        encounter = encounter->NextSiblingElement();
    }

    if(encounter != NULL) {
        int type = atoi(encounter-> Attribute("type"));
        std::string storybit = encounter -> FirstChildElement("storybit")->GetText();
        std::string option = encounter -> FirstChildElement("option")->GetText();

        return new Encounter(storybit, option, type);
    }
}

Encounter* EncounterRepository::getRandomEncounter() {
    return loadRandomEncounterFromXML();
}
