#include "gamestatemanager.h"
#include "tinyxml2.h"
#include "scene.h"
#include "choice.h"
#include <iostream>
#include <vector>

using namespace tinyxml2;

const char* STORY_PATH = "story/story.xml";

GameStateManager::GameStateManager() {};

Chapter* GameStateManager::loadChapterFromXML(int chapterIndex) {
    XMLDocument doc;
    doc.LoadFile(STORY_PATH);
    XMLElement * storyRoot = doc.RootElement();
    if (storyRoot != NULL) {
        XMLElement * chapter = storyRoot -> FirstChildElement("chapter");
        int actChapterIndex = 1;
        while(chapter != NULL && chapterIndex != actChapterIndex) {
            chapter = chapter->NextSiblingElement();
            if(chapter != NULL) {
                actChapterIndex = atoi(chapter -> Attribute("order"));
            }
        }
        if(chapter != NULL) {
            int cOrder = atoi(chapter -> Attribute("order"));
            std::string title = chapter -> Attribute("title");
            std::vector<Scene*> scenes = {};
            for (XMLElement* scene = chapter->FirstChildElement(); scene != NULL; scene = scene->NextSiblingElement()) {
                     XMLElement * artChild = scene -> FirstChildElement("art");
                     XMLElement * storyChild = scene -> FirstChildElement("storybit");
                     XMLElement * options = scene -> FirstChildElement("options");

                    std::string order = scene -> Attribute("order");
                    std::string art= artChild->GetText();
                    std::string storybit = storyChild -> GetText();
                    std::vector<Choice*> choices = {};

                    for (XMLElement* choice = options->FirstChildElement(); choice != NULL; choice = choice->NextSiblingElement())
                    {
                        int diff = atoi(choice -> Attribute("diff"));
                        int attr = atoi(choice -> Attribute("attr"));
                        int exp = atoi(choice -> Attribute("exp"));
                        int cpn = atoi(choice -> Attribute("cpn"));
                        int step = atoi(choice -> Attribute("step"));
                        std::string txt = choice->GetText();
                        choices.push_back(new Choice(diff, cpn, txt, exp, attr, step));
                    }
                    scenes.push_back(new Scene(storybit, art, order, choices));
            }
            return new Chapter(cOrder, title, scenes);
        }
    }
    return nullptr;
}
