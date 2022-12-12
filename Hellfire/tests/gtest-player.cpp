#include <gtest/gtest.h>

#include "player.h"
#include <string>


class PlayerTest : public ::testing::Test{
    protected:
    void SetUp() override {
        player1 = new Player();
        player2.setExperience(-1);
        player2.setExperience(10);
        player2.setSkillPoints(10);
        player2.setName("Dummy");
    }

    void TearDown() override {
        delete player1;
    }

    Player* player1;
    Player player2;
};

TEST_F(PlayerTest, DefaultCtor)
{
    EXPECT_EQ(player1->getExperience(),0);
    EXPECT_EQ(player1->getSkillPoints(),5);
    EXPECT_EQ(player1->getName(),"");
}

TEST_F(PlayerTest, PlayerSetters)
{
     EXPECT_EQ(player2.getSkillPoints(),15);
     EXPECT_EQ(player2.getExperience(),9);
     EXPECT_EQ(player2.getName(),"");
}

TEST_F(PlayerTest, PlayerAttributes)
{
     EXPECT_EQ(player2.getAttributes()->getStrength(),0);
     player2.getAttributes()->upgradeStrength();
     EXPECT_EQ(player2.getAttributes()->getStrength(),1);
}

