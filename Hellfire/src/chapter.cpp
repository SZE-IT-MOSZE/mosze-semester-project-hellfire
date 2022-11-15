#include "chapter.h"

using namespace std;


Chapter::Chapter(int odr, std::string ttl, std::vector<Scene*> scns) : scenes(scns) {
    order = odr;
    title = ttl;
    actSceneIndex = 0;
}

/*Chapter& Chapter::operator=(Chapter& ch)  {
    title = ch.title;
    order = ch.order;
    scenes = ch.scenes;
    actSceneIndex = 0;

    return *this;
} */

int Chapter::getOrder() {
    return order;
}

int Chapter::getSceneCount() {
    return scenes.size();
}

std::string Chapter::getTitle() {
    return title;
}

bool Chapter::isFinished(){

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
}

Chapter::~Chapter() {
  for (auto p : scenes)
   {
     delete p;
   }
   scenes.clear();
}

void Chapter::nextScene() {
    if(actSceneIndex < scenes.size()) {
       actSceneIndex++;
    }
}



