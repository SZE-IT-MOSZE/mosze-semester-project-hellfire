#include <gtest/gtest.h>
//#include "game.h"

#include <string>


class GameTest : public ::testing::Test{
    protected:
    void SetUp() override {
        //Game::newGame();

    }


};

TEST_F(GameTest, SampleTest)
{
    EXPECT_EQ(1,1);
    EXPECT_NE(1,2);
}



