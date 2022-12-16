#include "scene.h"
#include "player.h"
#include <random>

Scene::Scene(std::string sb, std::string a,  std::string odr, std::vector<Choice*> chs) : choices(chs)
{
    storybit = sb;
    art = a;
    order = odr;
}

int Scene::d20numberGenerator()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1,20);
    int random_integer = uni(rng);
    return random_integer;
}

void Scene::attributeCheck(Choice* chosenChoice, Player* player)
{
    int choiceType = chosenChoice -> getType();
    Attributes* playerAttributes = player->getAttributes();
    int randomFactor = d20numberGenerator();
    int choiceDiff = chosenChoice->getDifficulty();
    Weapon* playerWeapon = player->getEquippedWeapon();
    int weaponType = -1;
    int weaponBuff = 0;
    if(playerWeapon != nullptr) {
        weaponType = playerWeapon->getType();
        weaponBuff = playerWeapon->getEffectiveness();
    }
    int buffType = player->getBuffType();
    int buff = player->getBuff();

    if(chosenChoice -> getType() == static_cast<int>(ChoiceType::Default))
    {
        player->setExperience(chosenChoice->getExperience());
        playerAttributes->setCorruption(chosenChoice->getCorruption());
        return;
    }

    if(choiceType == static_cast<int>(ChoiceType::Strength))
    {
        int strength = playerAttributes->getStrength();
        if(buffType == static_cast<int>(ConsumableType::Strength))
            strength += buff;
        if(weaponType == static_cast<int>(WeaponType::Sword))
            strength += weaponBuff;

        if(randomFactor == 20 && choiceDiff / 2 <= strength)
        {
            player->setExperience(chosenChoice->getExperience());
            playerAttributes->setCorruption(chosenChoice->getCorruption());
            return;
        }
        else if(choiceDiff <= strength)
        {
            player->setExperience(chosenChoice->getExperience());
            playerAttributes->setCorruption(chosenChoice->getCorruption());
            return;
        }
        else
        {
            chosenChoice->setFailed();
            return;
        }
    }

    else if(choiceType == static_cast<int>(ChoiceType::Intelligence))
    {
        int intelligence = playerAttributes->getIntelligence();
        if(buffType == static_cast<int>(ConsumableType::Intelligence))
            intelligence += buff;
        if(weaponType == static_cast<int>(WeaponType::Staff))
            intelligence += weaponBuff;
        if(randomFactor == 20 && choiceDiff / 2 <= intelligence)
        {
            player->setExperience(chosenChoice->getExperience());
            playerAttributes->setCorruption(chosenChoice->getCorruption());
            return;
        }
        else if(choiceDiff <= intelligence)
        {
            player->setExperience(chosenChoice->getExperience());
            playerAttributes->setCorruption(chosenChoice->getCorruption());
            return;
        }
        else
        {
            chosenChoice->setFailed();
            return;
        }
    }
    else
    {
        int persuasion = playerAttributes->getPersuasion();
        if(buffType == static_cast<int>(ConsumableType::Persuasion))
            persuasion += buff;
        if(randomFactor == 20 && choiceDiff / 2 <= persuasion)
        {
            player->setExperience(chosenChoice->getExperience());
            playerAttributes->setCorruption(chosenChoice->getCorruption());
            return;
        }
        else if(choiceDiff <= persuasion)
        {
            player->setExperience(chosenChoice->getExperience());
            playerAttributes->setCorruption(chosenChoice->getCorruption());
            return;
        }
        else
        {
            chosenChoice->setFailed();
            return;
        }
    }
}

void Scene::chooseChoice(Choice* chosenChoice, Player* player)
{
    if(chosenChoice -> isFailed())
    {
        return;
    }

    attributeCheck(chosenChoice, player);
    player->removeBuff();
}

std::string Scene::getArt()
{
    return art;
}

std::string Scene::getStorybit()
{
    return storybit;
}

std::string Scene::getOrder()
{
    return order;
}

std::vector<Choice*>& Scene::getChoices()
{
    return choices;
}

Scene::~Scene()
{
    for (auto p : choices)
    {
        delete p;
    }
    choices.clear();
}
