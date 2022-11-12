#include "Game.h"
#include "tinyxml2.h"
#include <vector>
#include "choice.h"
#include "scene.h"

using namespace tinyxml2;

const char* Game::STORY_PATH = "story/story.xml";

std::string init = "";
std::vector<Scene> initScns = {};
Chapter Game::actualChapter = Chapter(-1, init, initScns);



Game::Game(){

}

Chapter& Game::getChapter(){
    return actualChapter;
}

Chapter Game::loadNextChapter(){

}

Player& Game::getPlayer(){

}

void Game::loadGame(std::string name){

}

void Game::saveHame(std::string name){

}

void Game::openInventory(){

}

void Game::openAttribueWindow(){

}

void Game::openMenu() {

}

void Game::newGame(){
    XMLDocument doc;
    doc.LoadFile(STORY_PATH);
    XMLElement * storyRoot = doc.RootElement();
    if (storyRoot != NULL) {
        XMLElement * chapterOne = storyRoot -> FirstChildElement("chapter");
        if(chapterOne != NULL) {
            int cOrder = atoi(chapterOne -> Attribute("order"));
            std::string title = chapterOne -> Attribute("title");
            std::vector<Scene> scenes = {};
            for (XMLElement* scene = chapterOne->FirstChildElement(); scene != NULL; scene = scene->NextSiblingElement()) {
                     XMLElement * artChild = scene -> FirstChildElement("art");
                     XMLElement * storyChild = scene -> FirstChildElement("storybit");
                     XMLElement * options = scene -> FirstChildElement("options");

                    std::string order = scene -> Attribute("order");
                    std::string art= artChild->GetText();
                    std::string storybit = storyChild -> GetText();
                    std::vector<Choice> choices = {};
                    for (XMLElement* choice = options->FirstChildElement(); choice != NULL; choice = choice->NextSiblingElement())
                    {
                        int diff = atoi(choice -> Attribute("diff"));
                        int attr = atoi(choice -> Attribute("attr"));
                        int exp = atoi(choice -> Attribute("exp"));
                        int cpn = atoi(choice -> Attribute("cpn"));
                        int step = atoi(choice -> Attribute("step"));
                        std::string txt = choice->GetText();
                        choices.push_back(Choice(diff, cpn, txt, exp, attr, step));

                    }
                    scenes.push_back(Scene(storybit, art, order, choices));
            }
            Chapter act = Chapter(cOrder, title, scenes);
            actualChapter = act;
        }
    }

}

void Game::turn(){

}


