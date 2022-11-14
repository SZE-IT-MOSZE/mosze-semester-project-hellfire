#include "chapter.h"

using namespace std;

Chapter::Chapter() : scenes(vector<Scene>() = {}) {
    order = -1;
    title = "";
}

Chapter::Chapter(int odr, std::string ttl, std::vector<Scene>& scns) : scenes(scns) {
    order = odr;
    title = ttl;
}

Chapter& Chapter::operator=(Chapter& ch) {
    title = ch.title;
    order = ch.order;
    scenes = vector<Scene>(ch.scenes);
    actSceneIndex = 0;

    return *this;
}

int Chapter::getOrder() {
    return order;
}

std::string Chapter::getTitle() {
    return title;
}

bool Chapter::isFinished(){

}

void Chapter::drawScene() {

}

Scene* Chapter::nextScene() {
    if(actSceneIndex < scenes.size()) {
        return  &scenes[actSceneIndex++];
    }
    return NULL;
}



