#include <gtest/gtest.h>
#include "gamestatemanager.h"
#include "chapter.h"
#include "player.h"
#include <string>


class SaveLoadTest : public ::testing::Test{
    protected:
        GameStateManager gsm;
        Player* TestPlayer;
        Chapter* TestChapter;

    void SetUp() override {
        TestPlayer = new Player();
    }

    void TearDown() override {
    }
};

/*TEST_F(SaveLoadTest, saveFromGame)
{
    EXPECT_NE(gsm.loadGameStateFromXML("gameState.xml",TestPlayer,TestChapter),false);
}*/

TEST_F(SaveLoadTest, fileUsing)
{
    Chapter* LoadedChapter = gsm.loadChapterFromXML(1);

    TestPlayer->setExperience(99);

    gsm.saveGameStateToXML("testSave.xml",TestPlayer,LoadedChapter->getOrder(),LoadedChapter->getSceneIndex(),&LoadedChapter->getActScene()->getChoices());//Player* player, int chapterIndex, int sceneIndex, std::vector<Choice*>* choices);

    Chapter* chapterFromXML  = nullptr;
    EXPECT_EQ(gsm.loadGameStateFromXML("testSave.xml",TestPlayer,chapterFromXML),true);
    TestPlayer = gsm.getPlayerState();


    EXPECT_EQ(TestPlayer->getExperience(),99);
    EXPECT_EQ(TestPlayer->getSkillPoints(),5);
}

