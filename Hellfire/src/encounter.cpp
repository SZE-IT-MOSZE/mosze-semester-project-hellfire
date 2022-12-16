#include "encounter.h"
#include "itemrepository.h"
#include "item.h"
#include "weapon.h"

Encounter::Encounter(std::string sb, std::string opt, int tp) {
    storybit = sb;
    option = opt;
    type = tp;
}

std::string Encounter::getStorybit() {
    return storybit;
}

std::string Encounter::getOption() {
    return option;
}

int Encounter::getType()
{
    return type;
}

int Encounter::d20numberGenerator()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1,20);
    int random_integer = uni(rng);
    return random_integer;
}

//Random encounter esetén is kell attribute check
void Encounter::attributeCheck(int difficulty, Player* player)
{
    Attributes* playerAttributes = player->getAttributes();
    int randomFactor = d20numberGenerator();
    Weapon* playerWeapon = player->getEquippedWeapon();
    int weaponType = -1;
    int weaponBuff = 0;
    if(playerWeapon != nullptr) {
        weaponType = playerWeapon->getType();
        weaponBuff = playerWeapon->getEffectiveness();
    }
    int buffType = player->getBuffType();
    int buff = player->getBuff();

    //Ha az encounter típusa strength akkor a teljesítésével 500xp-t lehet szerezni
    if(type == static_cast<int>(ChoiceType::Strength))
    {
        int strength = playerAttributes->getStrength();
        if(buffType == static_cast<int>(ConsumableType::Strength))
            strength += buff;
        if(weaponType == static_cast<int>(WeaponType::Sword))
            strength += weaponBuff;

        if(randomFactor == 20 && difficulty / 2 <= strength)
        {
            player->setExperience(500);
            return;
        }
        else if(difficulty <= strength)
        {
            player->setExperience(500);
            return;
        }
    }

    //Ha az encounter típusa intelligence akkor a teljesítésével 150xp-t és egy random itemet lehet szerezni
    else if(type == static_cast<int>(ChoiceType::Intelligence))
    {
        int intelligence = playerAttributes->getIntelligence();
        if(buffType == static_cast<int>(ConsumableType::Intelligence))
            intelligence += buff;
        if(weaponType == static_cast<int>(WeaponType::Staff))
            intelligence += weaponBuff;
        if(randomFactor == 20 && difficulty / 2 <= intelligence)
        {
            ItemRepository itemRepository;
            Inventory* playerInventory = player->getInventory();
            player->setExperience(150);
            playerInventory->addItem(itemRepository.getRandomItem());
            return;
        }
        else if(difficulty <= intelligence)
        {
            ItemRepository itemRepository;
            Inventory* playerInventory = player->getInventory();
            playerInventory->addItem(itemRepository.getRandomItem());
            player->setExperience(150);
            return;
        }
    }
    //Ha az encounter típusa persuasion akkor a teljesítésével egy random itemet lehet szerezni
    else
    {
        int persuasion = playerAttributes->getPersuasion();
        if(buffType == static_cast<int>(ConsumableType::Persuasion))
            persuasion += buff;
        if(randomFactor == 20 && difficulty / 2 <= persuasion)
        {
            ItemRepository itemRepository;
            Inventory* playerInventory = player->getInventory();
            playerInventory->addItem(itemRepository.getRandomItem());
            return;
        }
        else if(difficulty <= persuasion)
        {
            ItemRepository itemRepository;
            Inventory* playerInventory = player->getInventory();
            playerInventory->addItem(itemRepository.getRandomItem());
            return;
        }
    }
}

void Encounter::engage(int difficulty, Player* player) {
    attributeCheck(difficulty, player);
}
