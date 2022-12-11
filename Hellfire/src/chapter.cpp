#include "chapter.h"

using namespace std;


Chapter::Chapter(int odr, std::string ttl, std::vector<Scene*> scns) : scenes(scns) {
    order = odr;
    title = ttl;
    actSceneIndex = 0;
}

int Chapter::getOrder() {
    return order;
}

int Chapter::getSceneCount() {
    return scenes.size();
}

std::string Chapter::getTitle() {
    return title;
}

void Chapter::drawScene() {

}

int Chapter::getSceneIndex() {
    return actSceneIndex;
}

Scene* Chapter::getActScene() {
    if(actSceneIndex < scenes.size()) {
        return scenes[actSceneIndex];
     }
     return nullptr;
}

Chapter::~Chapter() {
  for (auto p : scenes)
   {
     delete p;
   }
   scenes.clear();
}

void Chapter::nextScene(int step) {
    if(actSceneIndex < scenes.size()) {
       actSceneIndex += step;
    }
}



