#include "gtest/gtest.h"
#include <vector>
#include <string>
#include "chapter.h"
#include "scene.h"
#include "choice.h"
#include "player.h"

#include "gamestatemanager.h"

class StoryTest : public ::testing::Test{
    protected:



    Player* TestPlayer;
    Chapter* TestChapter;
    Chapter* LoadedChapter;
    Scene* scene1;
    Scene* scene2;
    Scene* scene3;
    Choice* choice1;
    Choice* choice2;
    Choice* choice3;
    Choice* choice4;
    Choice* choice5;


    void SetUp() override {
        TestPlayer = new Player();
        TestPlayer->setExperience(0);
        TestPlayer->setSkillPoints(0);

        choice1 = new Choice(0,0,"valasztas1",10,0,1);
        choice2 = new Choice(10,0,"valasztas2",20,0,1);
        choice3 = new Choice(0,0,"valasztas3",5,0,1);
        choice4 = new Choice(0,0,"valasztas4",100,0,1);
        choice5 = new Choice(0,0,"valasztas5",200,0,1);
        scene1 = new Scene("story bit 1","art1","1",{choice1,choice2});
        scene2 = new Scene("story bit 2","art2","2",{choice3});
        scene3 = new Scene("story bit 3","art3","3",{choice4,choice5});
        std::vector<Scene*> scenes = {scene1,scene2,scene3};
        TestChapter = new Chapter(99,"title",scenes);



        GameStateManager* gsm = new GameStateManager();
        LoadedChapter = gsm->loadChapterFromXML(1);

    }
};
/*  <scene order="1">
            <art>assets/Mordor1.bmp</art>
            <storybit>$p elérte Mordor sötét, lápos vidékeit, a levegõben forró hamu száll és a lelkét nyomó sötétség lassacskán erõsödik, ahogy a közeledik a fekete föld kapujához. A kapu elõtt sátor áll, melyet az erdõtündék õrsége lakott. Az õrség parancsnoka meglátja a kapu felé sétálni és megállítja:
            </storybit>
            <options>
            <option diff="0" attr="def" exp="10" cpn="0" step="1">Tovább</option>
            </options>
        </scene> */

TEST_F(StoryTest, ManualyLoadedChapterTest)
{
    EXPECT_EQ(TestChapter->getOrder(),99);
    EXPECT_EQ(TestChapter->getSceneCount(),3);
    TestChapter->nextScene(1);
    EXPECT_EQ(TestChapter->getSceneIndex(),1);
    TestChapter->nextScene(1);
    EXPECT_EQ(TestChapter->getSceneIndex(),2);
    TestChapter->nextScene(1);
    EXPECT_EQ(TestChapter->getSceneIndex(),3);

}

TEST_F(StoryTest, ManualyLoadedChapterTest_Experience)
{

    for (int i = 0;i<= 2;i++)
    {
        Scene* actScene = TestChapter->getActScene();
        //EXPECT_EQ(actScene->getOrder(),1);

        actScene->chooseChoice((actScene->getChoices()[0]), TestPlayer);

        TestChapter->nextScene(1);
        EXPECT_EQ(TestChapter->getSceneIndex(),i+1);

    }

    EXPECT_EQ(TestPlayer->getExperience(),115);
}

TEST_F(StoryTest, FileLoadedChapterTest)
{
    EXPECT_EQ(LoadedChapter->getOrder(),1);
    EXPECT_EQ(LoadedChapter->getSceneIndex(),0);
    LoadedChapter->nextScene(1);
    EXPECT_EQ(LoadedChapter->getSceneIndex(),1);
    EXPECT_EQ(LoadedChapter->getSceneCount(),17);

    Scene* actScene = LoadedChapter->getActScene();
    EXPECT_EQ(actScene->getOrder(),"2");


    actScene->chooseChoice((actScene->getChoices()[0]), TestPlayer);
    EXPECT_EQ(TestPlayer->getExperience(),10);



}


