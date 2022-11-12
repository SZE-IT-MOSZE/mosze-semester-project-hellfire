#include "scene.h"


Scene::Scene(std::string sb, std::string a,  std::string odr, std::vector<Choice> chs): choices(chs) {
    storybit = sb;
    art = a;
    order = odr;
}

void Scene::chooseChoice() {

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

std::vector<Choice>& Scene::getChoices() {
    return choices;
}
