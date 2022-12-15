#include "gamestatemanager.h"
#include <iostream>
#include <item.h>
#include <weapon.h>
#include <consumable.h>

using namespace tinyxml2;

const char* STORY_PATH = "story/story.xml";

GameStateManager::GameStateManager() {};

Chapter* GameStateManager::loadChapterFromXML(int chapterIndex)
{
    XMLDocument doc;
    doc.LoadFile(STORY_PATH);
    XMLElement * storyRoot = doc.RootElement();
    if (storyRoot != NULL)
    {
        XMLElement * chapter = storyRoot -> FirstChildElement("chapter");
        int actChapterIndex = 1;
        while(chapter != NULL && chapterIndex != actChapterIndex)
        {
            chapter = chapter->NextSiblingElement();
            if(chapter != NULL)
            {
                actChapterIndex = atoi(chapter -> Attribute("order"));
            }
        }
        if(chapter != NULL)
        {
            int cOrder = atoi(chapter -> Attribute("order"));
            std::string title = chapter -> Attribute("title");
            bool isLast = atoi(chapter -> Attribute("isLast"));
            std::vector<Scene*> scenes = {};
            for (XMLElement* scene = chapter->FirstChildElement(); scene != NULL; scene = scene->NextSiblingElement())
            {
                XMLElement * artChild = scene -> FirstChildElement("art");
                XMLElement * storyChild = scene -> FirstChildElement("storybit");
                XMLElement * options = scene -> FirstChildElement("options");

                std::string order = scene -> Attribute("order");
                std::string art= artChild->GetText();
                std::string storybit = " ";
                if(storyChild->GetText() != nullptr)
                    storybit = storyChild->GetText();
                std::vector<Choice*> choices = {};

                for (XMLElement* choice = options->FirstChildElement(); choice != NULL; choice = choice->NextSiblingElement())
                {
                    int diff = atoi(choice -> Attribute("diff"));
                    std::string attr = choice -> Attribute("attr");
                    int exp = atoi(choice -> Attribute("exp"));
                    int cpn = atoi(choice -> Attribute("cpn"));
                    int step = atoi(choice -> Attribute("step"));
                    std::string txt = choice->GetText();
                    choices.push_back(new Choice(diff, cpn, txt, exp, attr, step));
                }
                scenes.push_back(new Scene(storybit, art, order, choices));
            }
            return new Chapter(cOrder, title, scenes, isLast);
        }
    }
    return nullptr;
}

void GameStateManager::insertToXmlElement(std::string newElementName, std::string value, XMLNode* parent, XMLDocument* xmlDoc) {
    XMLElement * newElement = xmlDoc->NewElement(newElementName.c_str());
    newElement->SetText(value.c_str());
    parent->InsertEndChild(newElement);
}

void GameStateManager::insertToXmlElement(std::string newElementName, int value, XMLNode* parent, XMLDocument* xmlDoc) {
    XMLElement * newElement = xmlDoc->NewElement(newElementName.c_str());
    newElement->SetText(value);
    parent->InsertEndChild(newElement);
}

void GameStateManager::saveGameStateToXML(std::string filename, Player* player, int chapterIndex, int sceneIndex, std::vector<Choice*>* choices) {
    XMLDocument xmlState;
    XMLNode * root = xmlState.NewElement("state");
    xmlState.InsertFirstChild(root);

    XMLNode * pChapter = xmlState.NewElement("chapter");
    insertToXmlElement("chapter-index", chapterIndex, pChapter, &xmlState);
    insertToXmlElement("scene-index", sceneIndex, pChapter, &xmlState);
    for(int i = 0; i < choices->size(); i++) {
        std::string name = "choice-" + std::to_string(i);
        insertToXmlElement(name.c_str(), (*choices)[i]->isFailed(), pChapter, &xmlState);
    }

    XMLNode * pPlayer = xmlState.NewElement("player");
    Attributes* playerAttributes = player->getAttributes();

    insertToXmlElement("skill-points", player->getSkillPoints(), pPlayer, &xmlState);
    insertToXmlElement("experience", player->getExperience(), pPlayer, &xmlState);
    insertToXmlElement("strength", playerAttributes->getStrength(), pPlayer, &xmlState);
    insertToXmlElement("intelligence", playerAttributes->getIntelligence(), pPlayer, &xmlState);
    insertToXmlElement("persuasion", playerAttributes->getPersuasion(), pPlayer, &xmlState);
    insertToXmlElement("corruption", playerAttributes->getCorruption(), pPlayer, &xmlState);
    insertToXmlElement("buff", player->getBuff(), pPlayer, &xmlState);
    insertToXmlElement("buffType", player->getBuffType(), pPlayer, &xmlState);

    XMLNode * pInventory = xmlState.NewElement("inventory");
    Inventory* playerInventory = player->getInventory();
    std::vector<Item*>& items = playerInventory->getItems();
    for(int i = 0; i < playerInventory->getItemsCount(); i++) {
        Weapon* weapon = dynamic_cast<Weapon*>(items[i]);
        Consumable* potion = dynamic_cast<Consumable*>(items[i]);
        if(weapon != nullptr) {
            XMLNode* pItem = xmlState.NewElement("item");
            insertToXmlElement("itemType", "weapon", pItem, &xmlState);
            insertToXmlElement("isEquipped", weapon->isEquipped(), pItem, &xmlState);
            insertToXmlElement("type", weapon->getType(), pItem, &xmlState);
            insertToXmlElement("name", weapon->getBaseName(), pItem, &xmlState);
            insertToXmlElement("art", weapon->getBaseArt(), pItem, &xmlState);
            insertToXmlElement("effect", weapon->getEffectiveness(), pItem, &xmlState);
            pInventory->InsertEndChild(pItem);
        }
        else if(potion != nullptr) {
            XMLNode* pItem = xmlState.NewElement("item");
            insertToXmlElement("itemType", "consumable", pItem, &xmlState);
            insertToXmlElement("type", potion->getType(), pItem, &xmlState);
            insertToXmlElement("name", potion->getName(), pItem, &xmlState);
            insertToXmlElement("art", potion->getBaseArt(), pItem, &xmlState);
            insertToXmlElement("charges", potion->getCharges(), pItem, &xmlState);
            insertToXmlElement("effect", potion->getEffectiveness(), pItem, &xmlState);
            pInventory->InsertEndChild(pItem);
        }
    }

    root->InsertEndChild(pChapter);
    root->InsertEndChild(pPlayer);
    root->InsertEndChild(pInventory);

    xmlState.SaveFile(filename.c_str());
}

bool GameStateManager::loadGameStateFromXML(std::string filename, Player* player, Chapter* chapter) {
    XMLDocument xmlState;
    XMLError result = xmlState.LoadFile(filename.c_str());
    if(result != XML_SUCCESS) {
        return false;
    }
    XMLElement * root = xmlState.RootElement();
    XMLElement * pChapter = root -> FirstChildElement("chapter");
    XMLElement * pPlayer = root -> FirstChildElement("player");
    XMLElement * pInventory = root -> FirstChildElement("inventory");

    if(chapter != nullptr) {
        delete chapter;
    }
    chapterState = loadChapterFromXML(atoi(pChapter->FirstChildElement("chapter-index")->GetText()));
    chapterState ->setSceneIndex(atoi(pChapter->FirstChildElement("scene-index")->GetText()));
    std::vector<Choice*>& chapterChoices = chapterState->getActScene()->getChoices();
    for(int i = 0; i < chapterChoices.size(); i++) {
        std::string choiceName = "choice-" + std::to_string(i);
        if(atoi(pChapter->FirstChildElement(choiceName.c_str())->GetText())) {
            chapterChoices[i]->setFailed();
        }
    }
    if(player != nullptr) {
        delete player;
    }
    int skillPoints = atoi(pPlayer->FirstChildElement("skill-points")->GetText());
    int experience = atoi(pPlayer->FirstChildElement("experience")->GetText());
    int strength = atoi(pPlayer->FirstChildElement("strength")->GetText());
    int intelligence = atoi(pPlayer->FirstChildElement("intelligence")->GetText());
    int persuasion = atoi(pPlayer->FirstChildElement("persuasion")->GetText());
    int corruption = atoi(pPlayer->FirstChildElement("corruption")->GetText());
    int buff = atoi(pPlayer->FirstChildElement("buff")->GetText());
    int buffType = atoi(pPlayer->FirstChildElement("buffType")->GetText());

    Attributes* attributes = new Attributes(strength, intelligence, persuasion, corruption);

    Inventory* inventory = new Inventory();
    Weapon* equippedWeapon = nullptr;
    for (XMLElement* pItem = pInventory->FirstChildElement(); pItem != NULL; pItem = pItem->NextSiblingElement()) {
        std::string itemType = pItem->FirstChildElement("itemType")->GetText();
        if(itemType == "weapon") {
           bool equipped = atoi(pItem->FirstChildElement("isEquipped")->GetText());
           int type = atoi(pItem->FirstChildElement("type")->GetText());
           std::string name = pItem->FirstChildElement("name")->GetText();
           std::string art = pItem->FirstChildElement("art")->GetText();
           int effect = atoi(pItem->FirstChildElement("effect")->GetText());
           Weapon* actWeapon = new Weapon(name, art, getWeaponTypeText(type), effect);
           if(equipped) {
            equippedWeapon = actWeapon;
           }
           inventory->addItem(actWeapon);
        }
        else if(itemType == "consumable") {
           int type = atoi(pItem->FirstChildElement("type")->GetText());
           std::string name = pItem->FirstChildElement("name")->GetText();
           std::string art = pItem->FirstChildElement("art")->GetText();
           int charges = atoi(pItem->FirstChildElement("charges")->GetText());
           int effect = atoi(pItem->FirstChildElement("effect")->GetText());
           Consumable* actConsumable = new Consumable(name, art, getConsumableTypeText(type), charges, effect);
           inventory->addItem(actConsumable);
        }
    }

    playerState = new Player(skillPoints, experience, buff, buffType, attributes, inventory, equippedWeapon);

    return true;
}

Player* GameStateManager::getPlayerState() {
    return playerState;
}

Chapter* GameStateManager::getChapterState() {
    return chapterState;
}

