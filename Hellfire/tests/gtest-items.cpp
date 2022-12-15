#include <gtest/gtest.h>

#include "itemrepository.h"
#include <string>


class ItemsTest : public ::testing::Test{
    protected:
        Item* randomItem;
    void SetUp() override {
        ItemRepository* itemRepository;
        randomItem = itemRepository->getRandomItem();

    }

    void TearDown() override {

    }

};

TEST_F(ItemsTest, RandomItemTest)
{
    if (dynamic_cast<Weapon*>(randomItem) != nullptr) //weapon
    {
        EXPECT_EQ((randomItem->getType() == static_cast<int>(WeaponType::Sword)
                   || randomItem->getType() == static_cast<int>(WeaponType::Staff))
                  ,true);


    } else if (dynamic_cast<Consumable*>(randomItem) != nullptr) //consumable
    {
        EXPECT_EQ((randomItem->getType() == static_cast<int>(ConsumableType::Intelligence)
                   || randomItem->getType() == static_cast<int>(ConsumableType::Persuasion)
                   || randomItem->getType() == static_cast<int>(ConsumableType::Strength))
                  ,true);

    }
    else
    {
        EXPECT_EQ(randomItem,nullptr);
        return;
    }

    EXPECT_GE(randomItem->getEffectiveness(),0);


}
