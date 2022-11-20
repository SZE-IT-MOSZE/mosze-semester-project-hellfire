#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "Chapter.h"
#include "tinyxml2.h"
using namespace tinyxml2;

class StoryTest : public ::testing::Test{
    protected:

    Chapter* actualChapter;
    Chapter* getChapter();
    void setChapter(Chapter * newChapter);

    void SetUp() override {
        loadStory();
    }


    void loadStory(){
        XMLDocument doc;
        doc.LoadFile("story/story.xml");
        XMLElement * storyRoot = doc.RootElement();
        if (storyRoot != NULL) {
            XMLElement * chapterOne = storyRoot -> FirstChildElement("chapter");
            if(chapterOne != NULL) {
                int cOrder = atoi(chapterOne -> Attribute("order"));
                std::string title = chapterOne -> Attribute("title");
                std::vector<Scene*> scenes = {};
                for (XMLElement* scene = chapterOne->FirstChildElement(); scene != NULL; scene = scene->NextSiblingElement()) {
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
                actualChapter = new Chapter(cOrder, title, scenes);

            }
        }
    }

};

TEST_F(StoryTest, teszt)
{
    EXPECT_NE(actualChapter, nullptr);
}
