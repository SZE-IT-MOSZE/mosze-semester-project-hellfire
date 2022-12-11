#include "scene.h"
#include "player.h"
#include <random>

Scene::Scene(std::string sb, std::string a,  std::string odr, std::vector<Choice*> chs) : choices(chs) {
    storybit = sb;
    art = a;
    order = odr;
}

int Scene::d20numberGenerator() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1,20);
    int random_integer = uni(rng);
    return random_integer;
}


void Scene::chooseChoice(Choice* chosenChoice, Player* player) {
    std::cout << "Type: " << chosenChoice->getType() << std::endl;
    std::cout << "Diff: " << chosenChoice->getDifficulty() << std::endl;
    std::cout << "Exp: "<< chosenChoice->getExperience() << std::endl;
    std::cout << "Cpn: " << chosenChoice->getCorruption() << std::endl << std::endl;

    if(chosenChoice -> isFailed()) {
        return;
    }

    int choiceType = chosenChoice -> getType();

    if(chosenChoice -> getType() == static_cast<int>(ChoiceType::Default)) {
        player->setExperience(chosenChoice->getExperience());
        return;
    }

    int randomFactor = d20numberGenerator();

    int choiceDiff = chosenChoice->getDifficulty();
    Attributes* playerAttributes = player->getAttributes();

    if(choiceType == static_cast<int>(ChoiceType::Strength)) {
        if(randomFactor == 20 && choiceDiff / 2 <= playerAttributes->getStrength()) {
            return;
        }
        else if(choiceDiff <= playerAttributes->getStrength()) {
            return;
        }
        else {
            chosenChoice->setFailed();
            return;
        }
    }
    else if(choiceType == static_cast<int>(ChoiceType::Intelligence)) {
         if(randomFactor == 20 && choiceDiff / 2 <= playerAttributes->getIntelligence()) {
            return;
        }
        else if(choiceDiff <= playerAttributes->getIntelligence()) {
            return;
        }
        else {
            chosenChoice->setFailed();
            return;
        }
    }
    else {
        if(randomFactor == 20 && choiceDiff / 2 <= playerAttributes->getPersuasion()) {
            return;
        }
        else if(choiceDiff <= playerAttributes->getPersuasion()) {
            return;
        }
        else {
            chosenChoice->setFailed();
            return;
        }
    }
}

std::string Scene::getArt() {
    return art;
}

std::string Scene::getStorybit() {
    return storybit;
}

std::string Scene::getOrder() {
    return order;
}

std::vector<Choice*>& Scene::getChoices() {
    return choices;
}

Scene::~Scene() {
   for (auto p : choices)
   {
      delete p;
   }
   choices.clear();
}
