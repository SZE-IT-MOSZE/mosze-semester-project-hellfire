#include "scene.h"
#include "player.h"
#include "game.h"


Scene::Scene(std::string sb, std::string a,  std::string odr, std::vector<Choice*> chs) : choices(chs) {
    storybit = sb;
    art = a;
    order = odr;
}

void Scene::chooseChoice(Choice* chosenChoice) {
    std::cout << "Type: " << chosenChoice->getType() << std::endl;
    std::cout << "Diff: " << chosenChoice->getDifficulty() << std::endl;
    std::cout << "Exp: "<< chosenChoice->getExperience() << std::endl;
    std::cout << "Cpn: " << chosenChoice->getCorruption() << std::endl << std::endl;

    Player* player = Game::getPlayer();
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
