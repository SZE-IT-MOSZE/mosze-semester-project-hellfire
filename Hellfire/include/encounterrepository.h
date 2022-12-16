#ifndef ENCOUNTERREPOSITORY_H
#define ENCOUNTERREPOSITORY_H

#include "encounter.h"
#include "tinyxml2.h"
#include <random>

class EncounterRepository
{
    public:
        Encounter* getRandomEncounter();
        EncounterRepository();
    private:
       Encounter* loadRandomEncounterFromXML();
};

#endif // ENCOUNTERREPOSITORY_H

