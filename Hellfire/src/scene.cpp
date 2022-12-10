#include "scene.h"
#include "player.h"

Scene::Scene(std::string sb, std::string a,  std::string odr, std::vector<Choice*> chs) : choices(chs) {
    storybit = sb;
    art = a;
    order = odr;
}

void Scene::chooseChoice(Choice* chosenChoice, Player* player) {
    std::cout << "Type: " << chosenChoice->getType() << std::endl;
    std::cout << "Diff: " << chosenChoice->getDifficulty() << std::endl;
    std::cout << "Exp: "<< chosenChoice->getExperience() << std::endl;
    std::cout << "Cpn: " << chosenChoice->getCorruption() << std::endl << std::endl;

    player->setExperience(chosenChoice->getExperience());
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
