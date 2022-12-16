#include "game.h"
#include <vector>
#include "choice.h"
#include "scene.h"
#include "render.h"
#include "interaction.h"
#include "weapon.h"
#include "gamestatemanager.h"
#include "itemrepository.h"
#include "encounter.h"
#include "encounterrepository.h"

const char* Game::GAME_TITLE = "MOrdor'S Zealous Enmity: A Lord of the Rings story";

Chapter* Game::actualChapter = nullptr;
Player* Game::player = nullptr;


Game::Game() { }

void Game::saveGame()
{
    GameStateManager stateManager;
    stateManager.saveGameStateToXML("gameState.xml", player, actualChapter->getOrder(), actualChapter->getSceneIndex(), &actualChapter->getActScene()->getChoices());
}

bool Game::loadGame()
{
    GameStateManager stateManager;
    bool result = stateManager.loadGameStateFromXML("gameState.xml", player, actualChapter);
    if(result)
    {
        player = stateManager.getPlayerState();
        actualChapter = stateManager.getChapterState();
    }
    return result;
}

Chapter* Game::getChapter()
{
    return actualChapter;
}

//Új chapter betöltése xml-ből, egyszerre mindig egy chapter van betöltve
void Game::loadNextChapter(int chapterIndex)
{
    GameStateManager stateManager;
    if(actualChapter != nullptr)
    {
        delete actualChapter;
    }
    actualChapter = stateManager.loadChapterFromXML(chapterIndex);
}

Player* Game::getPlayer()
{
    return player;
}

void Game::showEncounter(TTF_Font* font, TTF_Font* titleFont) {
    //Képernyő törlése
    SDL_RenderClear(Render::renderer);
    SDL_Color color[2] = {{255,255,255}, {255,0,0}};
    SDL_Surface* title = TTF_RenderUTF8_Solid(titleFont, "Random encounter", color[0]);

    EncounterRepository encounterRepository;

    //encounterRepository-val egy random encounter betöltése
    Encounter* encounter = encounterRepository.getRandomEncounter();
    std::string storybitText = encounter->getStorybit();
    std::string optionText = encounter->getOption();
    //Az encounter difficulty scalelődik az adott chapter számával, hogy teljesíthető legyen de közben nehéz
    int difficulty = actualChapter->getOrder() * 5;
    int type = encounter->getType();
    if(type == static_cast<int>(ChoiceType::Strength)) {
        optionText += " (" + std::to_string(difficulty) + " strength)";
    }
    else if(type == static_cast<int>(ChoiceType::Intelligence)) {
        optionText += " (" + std::to_string(difficulty) + " intelligence)";
    }
    else {
        optionText += " (" + std::to_string(difficulty) + " persuasion)";
    }

    SDL_Surface* storybit = TTF_RenderUTF8_Solid_Wrapped(font, storybitText.c_str(), color[0], Render::WIDTH - 200);
    SDL_Surface* option = TTF_RenderUTF8_Solid_Wrapped(font, optionText.c_str(), color[1], Render::WIDTH - 200);

    SDL_Surface* background = SDL_LoadBMP("assets/Mordor2.bmp");
    SDL_Surface* dialogFrame = SDL_LoadBMP("assets/dialogframe.bmp");

    SDL_Rect pos_img = {  0,
                          0,
                          background->w, background->h
                       };

    SDL_Rect pos_frame = {  0,
                            420,
                            dialogFrame->w, dialogFrame->h
                         };

    SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                          Render::HEIGHT / 4 - title->clip_rect.h,
                          title->w, title->h
                         };
    SDL_Rect pos[2] =  { {
            Render::WIDTH / 2 - storybit->clip_rect.w/2,
            Render::HEIGHT / 2 - storybit->clip_rect.h,
            storybit->w, storybit->h
        },
        {
            Render::WIDTH / 2 - option->clip_rect.w/2,
            440,
            option->w, option->h
        },
    };


    Render::renderSurface(background, pos_img);
    Render::renderSurface(dialogFrame, pos_frame);
    Render::renderSurface(title, titlePos);
    Render::renderSurface(storybit, pos[0]);
    Render::renderSurface(option, pos[1]);

    Uint32 time;
    SDL_Event event;
    while(true)
    {
        time=SDL_GetTicks();
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                return;
            case SDL_KEYDOWN:
                //Enter hatására megpróbáljuk teljesíteni az encountert és visszajutunk a játékmenetbe
                if(SDL_SCANCODE_RETURN == event.key.keysym.scancode)
                {
                    encounter->engage(difficulty, player);
                    delete encounter;
                    return;
                }
            }
        }
    }


}

//Ez a menü játék közben megnyitható
int Game::openInGameMenu(TTF_Font* font)
{
    SDL_RenderClear(Render::renderer);

    //SDL-es elemek konfigurációja, kiírandó textek, color, pozíciók
    bool selected[2] = {true, false};
    SDL_Color color[2] = {{255,255,255}, {255,0,0}};
    SDL_Surface* background = SDL_LoadBMP("assets/main.bmp");
    SDL_Surface* title = TTF_RenderUTF8_Solid(font, "Menü", color[0]);
    SDL_Surface* menus[2] =  {TTF_RenderUTF8_Solid(font, "Mentés", color[1]),
                              TTF_RenderUTF8_Solid(font, "Játékmenet befejezése", color[0]),
                             };
    SDL_Rect pos[2] =  { {
            Render::WIDTH / 2 - menus[0]->clip_rect.w / 2,
            Render::HEIGHT / 2 - menus[0]->clip_rect.h,
            menus[0]->w, menus[0]->h
        },
        {
            Render::WIDTH / 2 - menus[1]->clip_rect.w / 2,
            Render::HEIGHT / 2 + menus[1]->clip_rect.h,
            menus[1]->w, menus[1]->h
        },
    };
    SDL_Rect pos_img = {  0,
                          0,
                          background->w, background->h
                       };
    SDL_Rect pos_title =  {Render::WIDTH / 2 - title->clip_rect.w/2,
                           Render::HEIGHT / 4 - title->clip_rect.h,
                           title->w, title->h
                          };

    Render::renderSurface(background, pos_img);
    Render::renderSurface(title, pos_title);
    for(int i = 0; i < 2; i++)
    {
        Render::renderSurface(menus[i], pos[i]);
    }

    Uint32 time;
    SDL_Event event;
    while(true)
    {
        time=SDL_GetTicks();
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_KEYDOWN:
                //Ha a menüben a játék befejezése ki van választva, és entert nyomunk akkor visszatért a főmenübe
                if(SDL_SCANCODE_RETURN == event.key.keysym.scancode)
                {
                    if(selected[1])
                    {
                        return 0;
                    }
                  //Egyébként meg elmenti a játék állását, és visszadob a játékmenetbe
                    else
                    {
                        saveGame();
                        return 1;
                    }
                }
                //Escape esetén visszadob a játékmentetbe, ezzel zárható be a menü ha nem akarunk választani semmit
                else if(SDLK_ESCAPE == event.key.keysym.sym)
                {
                    return 1;
                }
                //Menüpontok választása fel-le nyilakkal
                else if(SDLK_UP == event.key.keysym.sym)
                {
                    background = SDL_LoadBMP("assets/main.bmp");
                    title = TTF_RenderUTF8_Solid(font, "Menü", color[0]);
                    if(selected[0])
                    {
                        selected[0] = false;
                        selected[1] = true;
                        menus[0] = TTF_RenderUTF8_Solid(font, "Mentés", color[0]);
                        menus[1] =  TTF_RenderUTF8_Solid(font, "Játékmenet befejezése", color[1]);
                    }
                    else
                    {
                        selected[1] = false;
                        selected[0] = true;
                        menus[0] = TTF_RenderUTF8_Solid(font, "Mentés", color[1]);
                        menus[1] =  TTF_RenderUTF8_Solid(font, "Játékmenet befejezése", color[0]);
                    }
                    SDL_RenderClear(Render::renderer);
                    Render::renderSurface(background, pos_img);
                    Render::renderSurface(title, pos_title);
                    for(int i = 0; i < 2; i++)
                    {
                        Render::renderSurface(menus[i], pos[i]);
                    }
                }

                else if(SDLK_DOWN == event.key.keysym.sym)
                {
                    background = SDL_LoadBMP("assets/main.bmp");
                    title = TTF_RenderUTF8_Solid(font, "Menü", color[0]);
                    if(selected[1])
                    {
                        selected[1] = false;
                        selected[0] = true;
                        menus[0] = TTF_RenderUTF8_Solid(font, "Mentés", color[1]);
                        menus[1] =  TTF_RenderUTF8_Solid(font, "Játékmenet befejezése", color[0]);
                    }
                    else
                    {
                        selected[0] = false;
                        selected[1] = true;
                        menus[0] = TTF_RenderUTF8_Solid(font, "Mentés", color[0]);
                        menus[1] =  TTF_RenderUTF8_Solid(font, "Játékmenet befejezése", color[1]);
                    }
                    SDL_RenderClear(Render::renderer);
                    Render::renderSurface(background, pos_img);
                    Render::renderSurface(title, pos_title);
                    for(int i = 0; i < 2; i++)
                    {
                        Render::renderSurface(menus[i], pos[i]);
                    }
                }
            }
        }
    }
    return -1;
}

//Inventory ablak megnyitását, és kirajzolásáért felel ez a metódus, mindig 4 item rajzolódik ki egy időben
void Game::openInventory(TTF_Font* font)
{
    SDL_RenderClear(Render::renderer);

    //SDL-es elemek konfigurációja, kiírandó textek, color, pozíciók
    //Továbbá a játékos inventoryjának és tartalmának lekérdezése
    Inventory* playerInventory = player->getInventory();
    std::vector<Item*>& items = playerInventory->getItems();
    int actPage = 1;
    int actItemIndex = 0;
    bool selected[playerInventory->getItemsCount() - 1];
    selected[0] = true;
    for(int i = 1; i < playerInventory->getItemsCount(); i++)
    {
        selected[i] = false;
    }
    SDL_Color color = {255,255,255};
    SDL_Surface* details[4][2];
    for(int i = 0; i < 4 && i < playerInventory->getItemsCount(); i++)
    {
        Consumable* potion = dynamic_cast<Consumable*>(items[i]);
        Weapon* weapon = dynamic_cast<Weapon*>(items[i]);
        std::string detailTop = "";
        std::string detailBottom = "Effectiveness: ";
        if(potion != nullptr)
        {
            detailTop += potion->getName();
            detailBottom += std::to_string(potion->getEffectiveness()) +
                            "      "
                            + "Charges: " + std::to_string(potion->getCharges());
            details[i][0] =  TTF_RenderUTF8_Solid(font, detailTop.c_str(), color);
            details[i][1] =  TTF_RenderUTF8_Solid(font, detailBottom.c_str(), color);
        }
        else if(weapon != nullptr)
        {
            detailTop += weapon->getName();
            detailBottom += std::to_string(weapon->getEffectiveness());
            details[i][0] =  TTF_RenderUTF8_Solid(font, detailTop.c_str(), color);
            details[i][1] =  TTF_RenderUTF8_Solid(font, detailBottom.c_str(), color);
        }
    }

    int baseHeight = 120;
    SDL_Rect pos_details[4][2];
    SDL_Rect pos_frames[4] =  { {
            30,
            baseHeight,
            900, 100
        },
        {
            30,
            baseHeight + 130,
            900, 100
        },
        {
            30,
            baseHeight + 2*130,
            900, 100
        },
        {
            30,
            baseHeight + 3*130,
            900, 100
        }
    };
    SDL_Rect pos_imgs[4] = { {
            80,
            baseHeight + 18,
            64, 64
        },
        {
            80,
            baseHeight + 148,
            64, 64
        },
        {
            80,
            baseHeight + 2*130 + 18,
            64, 64
        },
        {
            80,
            baseHeight +  3*130 + 18,
            64, 64
        }
    };

    for(int i = 0; i < 4 && i < playerInventory->getItemsCount(); i++)
    {
        pos_details[i][0] =
        {
            200,
            baseHeight + 18 + i * 130,
            details[i][0]->w, details[i][0]->h
        };
        pos_details[i][1] =
        {
            200,
            baseHeight + 46 + i * 130,
            details[i][1]->w, details[i][1]->h
        };
    }

    SDL_Surface* title = TTF_RenderUTF8_Solid(font, "Inventory - Press D to delete or F to use", color);
    SDL_Rect pos_title =  {Render::WIDTH / 2 - title->clip_rect.w/2,
                           Render::HEIGHT / 8 - title->clip_rect.h,
                           title->w, title->h
                          };

    //Meghatározzuk hogy hány page van az inventoryban ha egyszerre 4-et jelenítünk meg.
    std::string pageIndicatorText = std::to_string(actPage) + "/";
    if(playerInventory->getItemsCount() == 0)
    {
        pageIndicatorText += "1";
    }
    else
    {
        pageIndicatorText += std::to_string((playerInventory->getItemsCount() / 4 + (playerInventory->getItemsCount() % 4 != 0)));
    }
    SDL_Surface* pageIndicator = TTF_RenderUTF8_Solid(font, pageIndicatorText.c_str(), color);
    SDL_Rect pos_pageIndicator =  {Render::WIDTH - pageIndicator->clip_rect.w * 2,
                                   Render::HEIGHT - pageIndicator->clip_rect.h * 2,
                                   pageIndicator->w, pageIndicator->h
                                  };

    Render::renderSurface(title, pos_title);
    Render::renderSurface(pageIndicator, pos_pageIndicator);


    SDL_Surface* inventoryItemFrame = nullptr;
    SDL_Surface* selectedInventoryItemFrame = nullptr;
    SDL_Surface* inventoryItemIcon = nullptr;

    for(int i = (actPage - 1) * 4; i < 4 && i < playerInventory->getItemsCount(); i++)
    {
        inventoryItemIcon = SDL_LoadBMP(items[i]->getArt().c_str());
        //kezdetben a szelektált érték ugye a 0., és annak selectált állapotát más képpel jelezzük
        if(i == 0)
        {
            selectedInventoryItemFrame = SDL_LoadBMP("assets/selected_frame.bmp");
            Render::renderSurface(selectedInventoryItemFrame, pos_frames[i]);
        }
        else
        {
            inventoryItemFrame = SDL_LoadBMP("assets/frame.bmp");
            Render::renderSurface(inventoryItemFrame, pos_frames[i]);
        }
        Render::renderSurface(inventoryItemIcon, pos_imgs[i]);
        Render::renderSurface(details[i][0], pos_details[i][0]);
        Render::renderSurface(details[i][1], pos_details[i][1]);
    }
    Uint32 time;
    SDL_Event event;
    while(true)
    {
        time=SDL_GetTicks();
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                return;
            case SDL_KEYDOWN:
                //i gomb lenyomásával bezárhatjuk az inventory-t
                if(SDLK_i == event.key.keysym.sym)
                {
                    return;
                }
                //fel-le nyilak csak akkor renderelik újra az inventory-t ha nem üres
                else if(SDLK_UP == event.key.keysym.sym)
                {
                    if(actItemIndex > 0 && playerInventory->getItemsCount() > 0)
                    {
                        SDL_RenderClear(Render::renderer);
                        //felfelé nyíl hatására az actItemIndex csökken, az actItem index alapján pedig lemappeljük melyik pagen lehetünk ha
                        //egyszerre 4 item jelenik meg vizuálisan, indexelésnél is ügyelni kell erre
                        actItemIndex--;
                        selected[actItemIndex] = true;
                        selected[actItemIndex + 1] = false;
                        actPage = (actItemIndex + 1) / 4 + ((actItemIndex + 1) % 4 != 0);
                        title = TTF_RenderUTF8_Solid(font, "Inventory - Press D to delete or F to use", color);
                        std::string pageIndicatorText = std::to_string(actPage) + "/"
                                                        + std::to_string((playerInventory->getItemsCount() / 4 + (playerInventory->getItemsCount() % 4 != 0)));
                        pageIndicator = TTF_RenderUTF8_Solid(font, pageIndicatorText.c_str(), color);

                        Render::renderSurface(title, pos_title);
                        Render::renderSurface(pageIndicator, pos_pageIndicator);

                        for(int i = (actPage - 1) * 4; i < (4 + (actPage - 1) * 4) && i < playerInventory->getItemsCount(); i++)
                        {
                            //Attól függően hogy az adott item weapon, vagy consumable más módon rajzoljuk ki hiszen egy consumable-nek más tulajdonságai is vannak
                            //, mint pl a charges
                            Consumable* potion = dynamic_cast<Consumable*>(items[i]);
                            Weapon* weapon = dynamic_cast<Weapon*>(items[i]);
                            std::string detailTop = "";
                            std::string detailBottom = "Effectiveness: ";
                            if(potion != nullptr)
                            {
                                detailTop += potion->getName();
                                detailBottom += std::to_string(potion->getEffectiveness()) +
                                                "      "
                                                + "Charges: " + std::to_string(potion->getCharges());
                                details[i - 4 * (actPage - 1)][0] =  TTF_RenderUTF8_Solid(font, detailTop.c_str(), color);
                                details[i - 4 * (actPage - 1)][1] =  TTF_RenderUTF8_Solid(font, detailBottom.c_str(), color);
                            }
                            else if(weapon != nullptr)
                            {
                                detailTop += weapon->getName();
                                detailBottom += std::to_string(weapon->getEffectiveness());
                                details[i - 4 * (actPage - 1)][0] =  TTF_RenderUTF8_Solid(font, detailTop.c_str(), color);
                                details[i - 4 * (actPage - 1)][1] =  TTF_RenderUTF8_Solid(font, detailBottom.c_str(), color);
                            }
                            pos_details[i - 4 * (actPage - 1)][0] =
                            {
                                200,
                                baseHeight + 18 + (i - 4 * (actPage - 1)) * 130,
                                details[i - 4 * (actPage - 1)][0]->w, details[i - 4 * (actPage - 1)][0]->h
                            };
                            pos_details[i - 4 * (actPage - 1)][1] =
                            {
                                200,
                                baseHeight + 46 + (i - 4 * (actPage - 1)) * 130,
                                details[i - 4 * (actPage - 1)][1]->w, details[i - 4 * (actPage - 1)][1]->h
                            };
                            inventoryItemIcon = SDL_LoadBMP(items[i]->getArt().c_str());
                            if(selected[i])
                            {
                                selectedInventoryItemFrame = SDL_LoadBMP("assets/selected_frame.bmp");
                                Render::renderSurface(selectedInventoryItemFrame, pos_frames[i - 4 * (actPage - 1)]);
                            }
                            else
                            {
                                inventoryItemFrame = SDL_LoadBMP("assets/frame.bmp");
                                Render::renderSurface(inventoryItemFrame, pos_frames[i - 4 * (actPage - 1)]);
                            }
                            Render::renderSurface(inventoryItemIcon, pos_imgs[i - 4 * (actPage - 1)]);
                            Render::renderSurface(details[i - 4 * (actPage - 1)][0], pos_details[i - 4 * (actPage - 1)][0]);
                            Render::renderSurface(details[i - 4 * (actPage - 1)][1], pos_details[i - 4 * (actPage - 1)][1]);
                        }
                    }
                }

                else if(SDLK_DOWN == event.key.keysym.sym)
                {
                    if(actItemIndex < playerInventory->getItemsCount() - 1)
                    {
                        SDL_RenderClear(Render::renderer);
                        //lefelé nyíl hatására az actItemIndex nő
                        actItemIndex++;
                        selected[actItemIndex] = true;
                        selected[actItemIndex - 1] = false;
                        actPage = (actItemIndex + 1) / 4 + ((actItemIndex + 1) % 4 != 0);
                        title = TTF_RenderUTF8_Solid(font, "Inventory - Press D to delete or F to use", color);
                        std::string pageIndicatorText = std::to_string(actPage) + "/"
                                                        + std::to_string((playerInventory->getItemsCount() / 4 + (playerInventory->getItemsCount() % 4 != 0)));
                        pageIndicator = TTF_RenderUTF8_Solid(font, pageIndicatorText.c_str(), color);

                        Render::renderSurface(title, pos_title);
                        Render::renderSurface(pageIndicator, pos_pageIndicator);

                        for(int i = (actPage - 1) * 4; i < (4 + (actPage - 1) * 4) && i < playerInventory->getItemsCount(); i++)
                        {
                            Consumable* potion = dynamic_cast<Consumable*>(items[i]);
                            Weapon* weapon = dynamic_cast<Weapon*>(items[i]);
                            std::string detailTop = "";
                            std::string detailBottom = "Effectiveness: ";
                            if(potion != nullptr)
                            {
                                detailTop += potion->getName();
                                detailBottom += std::to_string(potion->getEffectiveness()) +
                                                "      "
                                                + "Charges: " + std::to_string(potion->getCharges());
                                details[i - 4 * (actPage - 1)][0] =  TTF_RenderUTF8_Solid(font, detailTop.c_str(), color);
                                details[i - 4 * (actPage - 1)][1] =  TTF_RenderUTF8_Solid(font, detailBottom.c_str(), color);
                            }
                            else if(weapon != nullptr)
                            {
                                detailTop += weapon->getName();
                                detailBottom += std::to_string(weapon->getEffectiveness());
                                details[i - 4 * (actPage - 1)][0] =  TTF_RenderUTF8_Solid(font, detailTop.c_str(), color);
                                details[i - 4 * (actPage - 1)][1] =  TTF_RenderUTF8_Solid(font, detailBottom.c_str(), color);
                            }
                            pos_details[i - 4 * (actPage - 1)][0] =
                            {
                                200,
                                baseHeight + 18 + (i - 4 * (actPage - 1)) * 130,
                                details[i - 4 * (actPage - 1)][0]->w, details[i - 4 * (actPage - 1)][0]->h
                            };
                            pos_details[i - 4 * (actPage - 1)][1] =
                            {
                                200,
                                baseHeight + 46 + (i - 4 * (actPage - 1)) * 130,
                                details[i - 4 * (actPage - 1)][1]->w, details[i - 4 * (actPage - 1)][1]->h
                            };
                            inventoryItemIcon = SDL_LoadBMP(items[i]->getArt().c_str());
                            if(selected[i])
                            {
                                selectedInventoryItemFrame = SDL_LoadBMP("assets/selected_frame.bmp");
                                Render::renderSurface(selectedInventoryItemFrame, pos_frames[i - 4 * (actPage - 1)]);
                            }
                            else
                            {
                                inventoryItemFrame = SDL_LoadBMP("assets/frame.bmp");
                                Render::renderSurface(inventoryItemFrame, pos_frames[i - 4 * (actPage - 1)]);
                            }
                            Render::renderSurface(inventoryItemIcon, pos_imgs[i - 4 * (actPage - 1)]);
                            Render::renderSurface(details[i - 4 * (actPage - 1)][0], pos_details[i - 4 * (actPage - 1)][0]);
                            Render::renderSurface(details[i - 4 * (actPage - 1)][1], pos_details[i - 4 * (actPage - 1)][1]);
                        }
                    }
                }
               //f gomb megnyomásával az adott kiválaszott itemet használhatjuk
               else if(SDLK_f == event.key.keysym.sym) {
                  for(int i = 0; i < playerInventory->getItemsCount(); i++) {
                    if(selected[i]) {
                        //itt is attól függően hogy consumable, vagy weapon más cselekmény következik be,
                        //ha weapon akkor equippelhetjük
                        //ha consumable akkor iszunk belőle, ezáltal csökken a charges értéke pl
                        Consumable* potion = dynamic_cast<Consumable*>(items[i]);
                        Weapon* weapon = dynamic_cast<Weapon*>(items[i]);
                        if(potion != nullptr) {
                            potion->use(player);
                        }
                        if(weapon != nullptr) {
                            player->setEquippedWeapon(weapon);
                        }
                        break;
                    }
                  }
                    //használat után újrarendereljük az inventory-t a friss állapotokkal
                    SDL_RenderClear(Render::renderer);
                    title = TTF_RenderUTF8_Solid(font, "Inventory - Press D to delete or F to use", color);
                    std::string pageIndicatorText = std::to_string(actPage) + "/";
                    if(playerInventory->getItemsCount() == 0)
                    {
                        pageIndicatorText += "1";
                    }
                    else
                    {
                        pageIndicatorText += std::to_string((playerInventory->getItemsCount() / 4 + (playerInventory->getItemsCount() % 4 != 0)));
                    }
                    pageIndicator = TTF_RenderUTF8_Solid(font, pageIndicatorText.c_str(), color);

                    Render::renderSurface(title, pos_title);
                    Render::renderSurface(pageIndicator, pos_pageIndicator);

                    for(int i = (actPage - 1) * 4; i < (4 + (actPage - 1) * 4) && i < playerInventory->getItemsCount(); i++)
                    {
                        Consumable* potion = dynamic_cast<Consumable*>(items[i]);
                        Weapon* weapon = dynamic_cast<Weapon*>(items[i]);
                        std::string detailTop = "";
                        std::string detailBottom = "Effectiveness: ";
                        if(potion != nullptr)
                        {
                            detailTop += potion->getName();
                            detailBottom += std::to_string(potion->getEffectiveness()) +
                                            "      "
                                            + "Charges: " + std::to_string(potion->getCharges());
                            details[i - 4 * (actPage - 1)][0] =  TTF_RenderUTF8_Solid(font, detailTop.c_str(), color);
                            details[i - 4 * (actPage - 1)][1] =  TTF_RenderUTF8_Solid(font, detailBottom.c_str(), color);
                        }
                        else if(weapon != nullptr)
                        {
                            detailTop += weapon->getName();
                            detailBottom += std::to_string(weapon->getEffectiveness());
                            details[i - 4 * (actPage - 1)][0] =  TTF_RenderUTF8_Solid(font, detailTop.c_str(), color);
                            details[i - 4 * (actPage - 1)][1] =  TTF_RenderUTF8_Solid(font, detailBottom.c_str(), color);
                        }
                        pos_details[i - 4 * (actPage - 1)][0] =
                        {
                            200,
                            baseHeight + 18 + (i - 4 * (actPage - 1)) * 130,
                            details[i - 4 * (actPage - 1)][0]->w, details[i - 4 * (actPage - 1)][0]->h
                        };
                        pos_details[i - 4 * (actPage - 1)][1] =
                        {
                            200,
                            baseHeight + 46 + (i - 4 * (actPage - 1)) * 130,
                            details[i - 4 * (actPage - 1)][1]->w, details[i - 4 * (actPage - 1)][1]->h
                        };
                        inventoryItemIcon = SDL_LoadBMP(items[i]->getArt().c_str());
                        if(selected[i])
                        {
                            selectedInventoryItemFrame = SDL_LoadBMP("assets/selected_frame.bmp");
                            Render::renderSurface(selectedInventoryItemFrame, pos_frames[i - 4 * (actPage - 1)]);
                        }
                        else
                        {
                            inventoryItemFrame = SDL_LoadBMP("assets/frame.bmp");
                            Render::renderSurface(inventoryItemFrame, pos_frames[i - 4 * (actPage - 1)]);
                        }
                        Render::renderSurface(inventoryItemIcon, pos_imgs[i - 4 * (actPage - 1)]);
                        Render::renderSurface(details[i - 4 * (actPage - 1)][0], pos_details[i - 4 * (actPage - 1)][0]);
                        Render::renderSurface(details[i - 4 * (actPage - 1)][1], pos_details[i - 4 * (actPage - 1)][1]);
                    }
               }
               //d hatására a kiválasztott item törlődik az inventory-ból
               else if(SDLK_d == event.key.keysym.sym) {
                  actPage = 1;
                  for(int i = 0; i < playerInventory->getItemsCount(); i++) {
                    if(selected[i]) {
                        selected[i] = false;
                        selected[0] = true;
                        actItemIndex = 0;
                        //ha weapon az adott item és equippelve volt akkor null pointerré tesszük a player equippedWeapon Weapon pointer tagváltozóját
                        Weapon* weapon = dynamic_cast<Weapon*>(items[i]);
                        if(weapon != nullptr) {
                            if(weapon->isEquipped()) {
                                player->setEquippedWeapon(nullptr);
                            }
                        }
                        playerInventory->deleteItem(items[i]);
                        break;
                    }
                  }
                  //Majd kirajzoljuk az inventory friss állapotát, nyilván a törölt elem nem fog megjelenni, az már a memóriából is törölve lett
                    SDL_RenderClear(Render::renderer);
                    title = TTF_RenderUTF8_Solid(font, "Inventory - Press D to delete or F to use", color);
                    std::string pageIndicatorText = std::to_string(actPage) + "/";
                    if(playerInventory->getItemsCount() == 0)
                    {
                        pageIndicatorText += "1";
                    }
                    else
                    {
                        pageIndicatorText += std::to_string((playerInventory->getItemsCount() / 4 + (playerInventory->getItemsCount() % 4 != 0)));
                    }
                    pageIndicator = TTF_RenderUTF8_Solid(font, pageIndicatorText.c_str(), color);

                    Render::renderSurface(title, pos_title);
                    Render::renderSurface(pageIndicator, pos_pageIndicator);

                    for(int i = (actPage - 1) * 4; i < (4 + (actPage - 1) * 4) && i < playerInventory->getItemsCount(); i++)
                    {
                        Consumable* potion = dynamic_cast<Consumable*>(items[i]);
                        Weapon* weapon = dynamic_cast<Weapon*>(items[i]);
                        std::string detailTop = "";
                        std::string detailBottom = "Effectiveness: ";
                        if(potion != nullptr)
                        {
                            detailTop += potion->getName();
                            detailBottom += std::to_string(potion->getEffectiveness()) +
                                            "      "
                                            + "Charges: " + std::to_string(potion->getCharges());
                            details[i - 4 * (actPage - 1)][0] =  TTF_RenderUTF8_Solid(font, detailTop.c_str(), color);
                            details[i - 4 * (actPage - 1)][1] =  TTF_RenderUTF8_Solid(font, detailBottom.c_str(), color);
                        }
                        else if(weapon != nullptr)
                        {
                            detailTop += weapon->getName();
                            detailBottom += std::to_string(weapon->getEffectiveness());
                            details[i - 4 * (actPage - 1)][0] =  TTF_RenderUTF8_Solid(font, detailTop.c_str(), color);
                            details[i - 4 * (actPage - 1)][1] =  TTF_RenderUTF8_Solid(font, detailBottom.c_str(), color);
                        }
                        pos_details[i - 4 * (actPage - 1)][0] =
                        {
                            200,
                            baseHeight + 18 + (i - 4 * (actPage - 1)) * 130,
                            details[i - 4 * (actPage - 1)][0]->w, details[i - 4 * (actPage - 1)][0]->h
                        };
                        pos_details[i - 4 * (actPage - 1)][1] =
                        {
                            200,
                            baseHeight + 46 + (i - 4 * (actPage - 1)) * 130,
                            details[i - 4 * (actPage - 1)][1]->w, details[i - 4 * (actPage - 1)][1]->h
                        };
                        inventoryItemIcon = SDL_LoadBMP(items[i]->getArt().c_str());
                        if(selected[i])
                        {
                            selectedInventoryItemFrame = SDL_LoadBMP("assets/selected_frame.bmp");
                            Render::renderSurface(selectedInventoryItemFrame, pos_frames[i - 4 * (actPage - 1)]);
                        }
                        else
                        {
                            inventoryItemFrame = SDL_LoadBMP("assets/frame.bmp");
                            Render::renderSurface(inventoryItemFrame, pos_frames[i - 4 * (actPage - 1)]);
                        }
                        Render::renderSurface(inventoryItemIcon, pos_imgs[i - 4 * (actPage - 1)]);
                        Render::renderSurface(details[i - 4 * (actPage - 1)][0], pos_details[i - 4 * (actPage - 1)][0]);
                        Render::renderSurface(details[i - 4 * (actPage - 1)][1], pos_details[i - 4 * (actPage - 1)][1]);
                    }
               }
            }
        }
    }
    return;
}

//az attribute windowban oszthatja szét a player a skill pontjait attribútumokra
void Game::openAttribueWindow(TTF_Font* font, TTF_Font* titleFont)
{
    //SDL-es elemek konfigurációja, kiírandó textek, color, pozíciók
    //továbbá a player attribútumainak lekérdezése
    SDL_RenderClear(Render::renderer);
    SDL_Color color[2] = {{255,255,255}, {255,0,0}};
    bool selected[3] = {true, false, false};
    Attributes* playerAttributes = player->getAttributes();
    SDL_Surface* corruptionArt = SDL_LoadBMP(player->determineCorruptionArt().c_str());
    std::string skillPointsText = "Szabad skill pontok: " + std::to_string(player->getSkillPoints());
    SDL_Surface* skillPoints = TTF_RenderUTF8_Solid(titleFont, skillPointsText.c_str(), color[0]);

    std::string strengthText = "Strength: " + std::to_string(playerAttributes->getStrength());
    std::string intelligenceText = "Intelligence: " + std::to_string(playerAttributes->getIntelligence());
    std::string persuasionText = "Persuasion: " + std::to_string(playerAttributes->getPersuasion());
    std::string corruptionText = "Corruption: " + std::to_string(playerAttributes->getCorruption());
    //ha a játékos rendelkezik buffal, melyet egy consumable elfogyasztásával szerzett azt külön jelezzük
    if(player->hasBuff()) {
        int type = player->getBuffType();
        if(type == static_cast<int>(ConsumableType::Strength)) {
            strengthText += " + " + std::to_string(player->getBuff());
        }
        else if(type == static_cast<int>(ConsumableType::Intelligence)) {
            intelligenceText += " + " + std::to_string(player->getBuff());
        }
        else {
            persuasionText += " + " + std::to_string(player->getBuff());
        }
    }
    //ha a játékosnak van egy equipped fegyvere azt külön jelezzük
    Weapon* playerWeapon = player->getEquippedWeapon();
    if(playerWeapon != nullptr) {
        int weaponType = playerWeapon->getType();
        if(weaponType == static_cast<int>(WeaponType::Sword)) {
             strengthText += " + " + std::to_string(playerWeapon->getEffectiveness());
        }
        else {
            intelligenceText += " + " + std::to_string(playerWeapon->getEffectiveness());
        }
    }

    SDL_Surface* strength = TTF_RenderUTF8_Solid(font, strengthText.c_str(), color[0]);
    SDL_Surface* intelligence = TTF_RenderUTF8_Solid(font, intelligenceText.c_str(), color[0]);
    SDL_Surface* persuasion = TTF_RenderUTF8_Solid(font, persuasionText.c_str(), color[0]);
    SDL_Surface* corruption = TTF_RenderUTF8_Solid(font, corruptionText.c_str(), color[0]);

    SDL_Surface* toggles[3] =
    {
        TTF_RenderUTF8_Solid(font, "Hozzáad", color[1]),
        TTF_RenderUTF8_Solid(font, "Hozzáad", color[0]),
        TTF_RenderUTF8_Solid(font, "Hozzáad", color[0]),
    };

    SDL_Rect img_pos =  {0 + corruptionArt->clip_rect.w,
                         0 + corruptionArt->clip_rect.h * 3,
                         corruptionArt->w, corruptionArt->h
                        };
    SDL_Rect cpnTextPos = {0 + corruptionArt->clip_rect.w,
                           0 + corruptionArt->clip_rect.h * 4,
                           corruption->w, corruption->h
                          };

    SDL_Rect titlePos =  {Render::WIDTH / 2 - skillPoints->clip_rect.w/2,
                          Render::HEIGHT / 4 - skillPoints->clip_rect.h,
                          skillPoints->w, skillPoints->h
                         };
    SDL_Rect pos[3] =  { {
            Render::WIDTH / 2 - strength->clip_rect.w,
            Render::HEIGHT / 2 - strength->clip_rect.h,
            strength->w, strength->h
        },
        {
            Render::WIDTH / 2 - intelligence->clip_rect.w,
            Render::HEIGHT / 2 + intelligence->clip_rect.h,
            intelligence->w, intelligence->h
        },
        {
            Render::WIDTH / 2 - persuasion->clip_rect.w,
            Render::HEIGHT / 2 + (persuasion->clip_rect.h * 3),
            persuasion->w, persuasion->h
        }
    };
    SDL_Rect togglePos[3] =
    {
        {
            Render::WIDTH / 2 + toggles[0]->clip_rect.w,
            Render::HEIGHT / 2 - strength->clip_rect.h,
            toggles[0]->w, toggles[0]->h
        },
        {
            Render::WIDTH / 2  + toggles[1]->clip_rect.w,
            Render::HEIGHT / 2 + intelligence->clip_rect.h,
            toggles[1]->w, toggles[1]->h
        },
        {
            Render::WIDTH / 2  + toggles[2]->clip_rect.w,
            Render::HEIGHT / 2 + (persuasion->clip_rect.h * 3),
            toggles[2]->w, toggles[2]->h
        }
    };

    Render::renderSurface(skillPoints, titlePos);
    Render::renderSurface(strength, pos[0]);
    Render::renderSurface(intelligence, pos[1]);
    Render::renderSurface(persuasion, pos[2]);
    Render::renderSurface(corruptionArt, img_pos);
    Render::renderSurface(corruption, cpnTextPos);
    for(int i = 0; i < 3; i++)
    {
        Render::renderSurface(toggles[i], togglePos[i]);
    }

    Uint32 time;
    SDL_Event event;
    while(true)
    {
        time=SDL_GetTicks();
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                return;
            case SDL_KEYDOWN:
                //Enter lenyomásával a kiválasztott attribútumhoz adhatjuk hozzá a skill pontjainkat
                if(SDL_SCANCODE_RETURN == event.key.keysym.scancode)
                {
                    SDL_RenderClear(Render::renderer);
                    for(int i = 0; i < 3; i++)
                    {
                        if(selected[i])
                        {
                            toggles[i] = TTF_RenderUTF8_Solid(font, "Hozzáad", color[1]);
                            if(player->getSkillPoints() > 0)
                            {
                                //Attól függően mely attribútumról van szó az adott upgrade metódus hívódik meg
                                //és természetesen csökken a skillpoint értéke 1-el
                                switch(i)
                                {
                                case 0:
                                    playerAttributes->upgradeStrength();
                                    player->setSkillPoints(-1);
                                    break;
                                case 1:
                                    playerAttributes->upgradeIntelligence();
                                    player->setSkillPoints(-1);
                                    break;
                                case 2:
                                    playerAttributes->upgradePersuasion();
                                    player->setSkillPoints(-1);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            toggles[i] = TTF_RenderUTF8_Solid(font, "Hozzáad", color[0]);
                        }
                        Render::renderSurface(toggles[i], togglePos[i]);
                    }
                    //friss állapotokat rendereljük utána
                    skillPointsText = "Szabad skill pontok: " + std::to_string(player->getSkillPoints());
                    skillPoints = TTF_RenderUTF8_Solid(titleFont, skillPointsText.c_str(), color[0]);

                    strengthText = "Strength: " + std::to_string(playerAttributes->getStrength());
                    intelligenceText = "Intelligence: " + std::to_string(playerAttributes->getIntelligence());
                    persuasionText = "Persuasion: " + std::to_string(playerAttributes->getPersuasion());
                    corruptionText = "Corruption: " + std::to_string(playerAttributes->getCorruption());
                    if(player->hasBuff()) {
                        int type = player->getBuffType();
                        if(type == static_cast<int>(ConsumableType::Strength)) {
                            strengthText += " + " + std::to_string(player->getBuff());
                        }
                        else if(type == static_cast<int>(ConsumableType::Intelligence)) {
                            intelligenceText += " + " + std::to_string(player->getBuff());
                        }
                        else {
                            persuasionText += " + " + std::to_string(player->getBuff());
                        }
                    }
                    if(playerWeapon != nullptr) {
                        int weaponType = playerWeapon->getType();
                        if(weaponType == static_cast<int>(WeaponType::Sword)) {
                             strengthText += " + " + std::to_string(playerWeapon->getEffectiveness());
                        }
                        else {
                            intelligenceText += " + " + std::to_string(playerWeapon->getEffectiveness());
                        }
                    }

                    strength = TTF_RenderUTF8_Solid(font, strengthText.c_str(), color[0]);
                    intelligence = TTF_RenderUTF8_Solid(font, intelligenceText.c_str(), color[0]);
                    persuasion = TTF_RenderUTF8_Solid(font, persuasionText.c_str(), color[0]);
                    corruption = TTF_RenderUTF8_Solid(font, corruptionText.c_str(), color[0]);

                    corruptionArt = SDL_LoadBMP(player->determineCorruptionArt().c_str());

                    Render::renderSurface(corruptionArt, img_pos);
                    Render::renderSurface(skillPoints, titlePos);
                    Render::renderSurface(strength, pos[0]);
                    Render::renderSurface(intelligence, pos[1]);
                    Render::renderSurface(persuasion, pos[2]);
                    Render::renderSurface(corruption, cpnTextPos);
                }
                //j lenyomásával visszatérünk a játékmenetbe
                else if(SDLK_j == event.key.keysym.sym)
                {
                    return;
                }
                //fel és le nyilak hatására kiválaszthatjuk melyik attribútumhoz szeretnénk hozzáadni egyet
                //a kiválasztottságot az adott hozzáad text piros színe indikálja
                else if(SDLK_UP == event.key.keysym.sym)
                {
                    for(int i = 0; i < 3; i++)
                    {
                        if(selected[i] == true)
                        {
                            selected[i] = false;

                            if(i == 0)
                            {
                                selected[2] = true;
                            }
                            else
                            {
                                selected[i-1] = true;
                            }
                            break;
                        }
                    }
                    SDL_RenderClear(Render::renderer);
                    strength = TTF_RenderUTF8_Solid(font, strengthText.c_str(), color[0]);
                    intelligence = TTF_RenderUTF8_Solid(font, intelligenceText.c_str(), color[0]);
                    persuasion = TTF_RenderUTF8_Solid(font, persuasionText.c_str(), color[0]);
                    skillPoints = TTF_RenderUTF8_Solid(titleFont, skillPointsText.c_str(), color[0]);
                    corruption = TTF_RenderUTF8_Solid(font, corruptionText.c_str(), color[0]);
                    corruptionArt = SDL_LoadBMP(player->determineCorruptionArt().c_str());

                    Render::renderSurface(corruptionArt, img_pos);
                    for(int i = 0; i < 3; i++)
                    {
                        if(selected[i])
                        {
                            toggles[i] = TTF_RenderUTF8_Solid(font, "Hozzáad", color[1]);
                        }
                        else
                        {
                            toggles[i] = TTF_RenderUTF8_Solid(font, "Hozzáad", color[0]);
                        }
                        Render::renderSurface(toggles[i], togglePos[i]);
                    }
                    Render::renderSurface(skillPoints, titlePos);
                    Render::renderSurface(strength, pos[0]);
                    Render::renderSurface(intelligence, pos[1]);
                    Render::renderSurface(persuasion, pos[2]);
                    Render::renderSurface(corruption, cpnTextPos);
                }

                else if(SDLK_DOWN == event.key.keysym.sym)
                {
                    for(int i = 0; i < 3; i++)
                    {
                        if(selected[i] == true)
                        {
                            selected[i] = false;

                            if(i == 2)
                            {
                                selected[0] = true;
                            }
                            else
                            {
                                selected[i+1] = true;
                            }
                            break;
                        }
                    }
                    SDL_RenderClear(Render::renderer);
                    strength = TTF_RenderUTF8_Solid(font, strengthText.c_str(), color[0]);
                    intelligence = TTF_RenderUTF8_Solid(font, intelligenceText.c_str(), color[0]);
                    persuasion = TTF_RenderUTF8_Solid(font, persuasionText.c_str(), color[0]);
                    skillPoints = TTF_RenderUTF8_Solid(titleFont, skillPointsText.c_str(), color[0]);
                    corruption = TTF_RenderUTF8_Solid(font, corruptionText.c_str(), color[0]);
                    corruptionArt = SDL_LoadBMP(player->determineCorruptionArt().c_str());

                    Render::renderSurface(corruptionArt, img_pos);
                    for(int i = 0; i < 3; i++)
                    {
                        if(selected[i])
                        {
                            toggles[i] = TTF_RenderUTF8_Solid(font, "Hozzáad", color[1]);
                        }
                        else
                        {
                            toggles[i] = TTF_RenderUTF8_Solid(font, "Hozzáad", color[0]);
                        }
                        Render::renderSurface(toggles[i], togglePos[i]);
                    }
                    Render::renderSurface(skillPoints, titlePos);
                    Render::renderSurface(strength, pos[0]);
                    Render::renderSurface(intelligence, pos[1]);
                    Render::renderSurface(persuasion, pos[2]);
                    Render::renderSurface(corruption, cpnTextPos);
                }
            }
        }
    }
}

//ez a főmenü mely a játék kezdetekor nyílik meg vagy a játékmenet közbeni menüből ide térhetünk vissza ha megszakítjuk a játékmenetet
int Game::openMenu(SDL_Surface* screen, TTF_Font* font, TTF_Font* titleFont, int previousState)
{

    //SDL-es elemek konfigurációja, kiírandó textek, color, pozíciók
    const int NUMMENU=3;
    const char* labels[NUMMENU] = {"Új játék", "Játék betöltése", "Kilépés"};
    SDL_Surface* menus[NUMMENU];
    bool selected[NUMMENU] = {0,0,0};
    selected[previousState] = true;
    SDL_Color color[2] = {{255,255,255}, {255,0,0}};
    SDL_RenderClear(Render::renderer);
    SDL_SetRenderDrawColor( Render::renderer, 0, 0, 0, 255 );
    SDL_Surface* background = SDL_LoadBMP("assets/main.bmp");
    SDL_Surface* title = TTF_RenderUTF8_Solid_Wrapped(titleFont, GAME_TITLE, color[0], Render::WIDTH - 450);

    for(int i = 0; i < NUMMENU; i++)
    {
        if(i == previousState)
        {
            menus[i] = TTF_RenderUTF8_Solid(font, labels[i], color[1]);
        }
        else
        {
            menus[i] = TTF_RenderUTF8_Solid(font, labels[i], color[0]);
        }
    }

    SDL_Rect pos[NUMMENU] = { {
            Render::WIDTH / 2 - menus[0]->clip_rect.w/2,
            Render::HEIGHT / 2 - menus[0]->clip_rect.h,
            menus[0]->w, menus[0]->h
        },
        {
            Render::WIDTH / 2 - menus[1]->clip_rect.w/2,
            Render::HEIGHT / 2 + menus[1]->clip_rect.h,
            menus[1]->w, menus[1]->h
        },
        {
            Render::WIDTH / 2 - menus[2]->clip_rect.w/2,
            Render::HEIGHT / 2 + (menus[2]->clip_rect.h * 3),
            menus[2]->w, menus[2]->h
        }
    };

    SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                          Render::HEIGHT / 4 - title->clip_rect.h + 50,
                          title->w, title->h
                         };

    SDL_Rect pos_img = {  0,
                          0,
                          background->w, background->h
                       };

    Render::renderSurface(background, pos_img);


    Render::renderSurface(title, titlePos);
    Render::renderSurface(menus[0], pos[0]);
    Render::renderSurface(menus[1], pos[1]);
    Render::renderSurface(menus[2], pos[2]);

    Interaction listener;
    int result = listener.listen([&selected, &menus, &font, &labels, &color, &pos](SDL_Event& event) -> int
    {
        switch(event.type)
        {
        case SDL_QUIT:
            return static_cast<int>(MenuOptions::EXIT_GAME);
        case SDL_KEYDOWN:
            //enter hatására visszaadjuk az adott kiválasztott érték indexét amit lekezelünk a main.cpp-ben
            if(SDL_SCANCODE_RETURN == event.key.keysym.scancode)
            {
                for(int i = 0; i < NUMMENU; i++)
                {
                    if(selected[i] == true)
                    {
                        return i;
                    }
                }
            }
            //fel-le nyilakkal ugrálhatnuk a menüpontok között, a változást újrarenderelés után az adott text piros színe indikálja
            else if(SDLK_UP == event.key.keysym.sym)
            {
                for(int i = 0; i < NUMMENU; i++)
                {
                    if(selected[i] == true)
                    {
                        selected[i] = false;
                        menus[i] = TTF_RenderUTF8_Solid(font, labels[i], color[0]);
                        Render::renderSurface(menus[i], pos[i]);

                        //mengézzük hogy ha pl felfelé ugrálás során a tetején vagyunk akkor a következő lenyomásra a legalsó pont lesz kiválasztva
                        if(i == 0)
                        {
                            selected[NUMMENU-1] = true;
                            menus[NUMMENU-1] = TTF_RenderUTF8_Solid(font, labels[NUMMENU-1], color[1]);
                            Render::renderSurface(menus[NUMMENU-1], pos[NUMMENU-1]);
                        }
                        else
                        {
                            selected[i-1] = true;
                            menus[i-1] = TTF_RenderUTF8_Solid(font, labels[i-1], color[1]);
                            Render::renderSurface(menus[i-1], pos[i-1]);
                        }
                        break;
                    }
                }
            }

            else if(SDLK_DOWN == event.key.keysym.sym)
            {
                for(int i = 0; i < NUMMENU; i++)
                {
                    if(selected[i] == true)
                    {
                        selected[i] = false;
                        menus[i] = TTF_RenderUTF8_Solid(font, labels[i], color[0]);
                        Render::renderSurface(menus[i], pos[i]);

                        if(i == NUMMENU-1)
                        {
                            selected[0] = true;
                            menus[0] = TTF_RenderUTF8_Solid(font, labels[0], color[1]);
                            Render::renderSurface(menus[0], pos[0]);
                        }
                        else
                        {
                            selected[i+1] = true;
                            menus[i+1] = TTF_RenderUTF8_Solid(font, labels[i+1], color[1]);
                            Render::renderSurface(menus[i+1], pos[i+1]);
                        }
                        break;
                    }
                }
            }
        }
    });
    return result;
}

//ez az ablak a chapterek között jelenik meg, megmutatja hány xp-t szereztünk, skill pontot, és itemet
void Game::openInterludeWindow(TTF_Font* titleFont, TTF_Font* font)
{
    SDL_RenderClear(Render::renderer);
    SDL_Color color[2] = {{255,255,255}, {255,0,0}};
    SDL_Surface* title = TTF_RenderUTF8_Solid(titleFont, "Fejezet vége", color[0]);

    ItemRepository itemRepository;

    int playerExperience = player->getExperience();
    std::string experienceBeforeConvertText = "Experience pontok átváltás előtt: " + std::to_string(playerExperience) + " exp";
    std::string experienceAfterConvertText = "Experience pontok átváltás után: " + std::to_string(playerExperience - (playerExperience / 100 * 100)) + " exp";
    std::string gainedSkillpointsText = "Szerzett skill pontok: " + std::to_string(playerExperience / 100);

    SDL_Surface* experienceBeforeConvert = TTF_RenderUTF8_Solid(font, experienceBeforeConvertText.c_str(), color[0]);
    SDL_Surface* experienceAfterConvert = TTF_RenderUTF8_Solid(font, experienceAfterConvertText.c_str(), color[0]);
    SDL_Surface* gainedSkillPoints = TTF_RenderUTF8_Solid(font, gainedSkillpointsText.c_str(), color[0]);
    SDL_Surface* itemDropInfo = TTF_RenderUTF8_Solid(font, "Kaptál 3 itemet, megnézheted őket az inventoryban.", color[0]);

    SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                          Render::HEIGHT / 4 - title->clip_rect.h,
                          title->w, title->h
                         };
    SDL_Rect pos[4] =  { {
            Render::WIDTH / 2 - experienceBeforeConvert->clip_rect.w/2,
            Render::HEIGHT / 2 - experienceBeforeConvert->clip_rect.h,
            experienceBeforeConvert->w, experienceBeforeConvert->h
        },
        {
            Render::WIDTH / 2 - experienceAfterConvert->clip_rect.w/2,
            Render::HEIGHT / 2 + experienceAfterConvert->clip_rect.h,
            experienceAfterConvert->w, experienceAfterConvert->h
        },
        {
            Render::WIDTH / 2 - gainedSkillPoints->clip_rect.w/2,
            Render::HEIGHT / 2 + (gainedSkillPoints->clip_rect.h * 3),
            gainedSkillPoints->w, gainedSkillPoints->h
        },
        {
            Render::WIDTH / 2 - itemDropInfo->clip_rect.w/2,
            Render::HEIGHT - 100,
            itemDropInfo->w, itemDropInfo->h
        }
    };


    Render::renderSurface(title, titlePos);
    Render::renderSurface(experienceBeforeConvert, pos[0]);
    Render::renderSurface(experienceAfterConvert, pos[1]);
    Render::renderSurface(gainedSkillPoints, pos[2]);
    Render::renderSurface(itemDropInfo, pos[3]);


    Uint32 time;
    SDL_Event event;
    while(true)
    {
        time=SDL_GetTicks();
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                return;
            case SDL_KEYDOWN:
                //Enter hatására bezárhatjuk az ablakot
                if(SDL_SCANCODE_RETURN == event.key.keysym.scancode)
                {
                    player->setExperience(-(playerExperience / 100 * 100));
                    player->setSkillPoints(playerExperience / 100);
                    Inventory* playerInventory = player->getInventory();
                    for(int i = 0; i < 3; i++)
                    {
                        playerInventory->addItem(itemRepository.getRandomItem());
                    }

                    int prevChapterOrder = actualChapter->getOrder();
                    loadNextChapter(prevChapterOrder + 1);
                    return;
                }
            }
        }
    }
}


//Ez a metódus új játékot hoz létre, ha már játszottunk, akkor törli az előző chaptert és playert és újra létrehozza őket a statikus tagváltozókba, betölti az első chaptert
void Game::newGame()
{
    if(player != nullptr)
    {
        delete player;
    }
    loadNextChapter(1);
    if(actualChapter != nullptr)
    {
        player = new Player();
    }
    else
    {
        std::cout << "Failed to load chapter from story.xml." << std::endl;
    }
}

//A játék legfontosabb metódusa, mely játékmenet közbeni interakciót kezeli le
int Game::turn(TTF_Font* font, TTF_Font* storyFont)
{
    SDL_RenderClear(Render::renderer);
    //SDL-es elemek konfigurációja, kiírandó textek, color, pozíciók
    //adott aktuális jelenet és hozzá tartozó choiceok lekérdezése
    Scene* actScene = actualChapter->getActScene();

    std::vector<Choice*>& choices =  actScene->getChoices();
    int playerCorruption = player->getAttributes()->getCorruption();

    //ending filterezése ha utolsó chapternél vagyunk corruption alapján
    if(actualChapter->isLastChapter() && actualChapter->getSceneIndex() == actualChapter->getSceneCount() - 1)
    {
        if(playerCorruption <= -3)
        {
            delete actScene->getChoices()[1];
            delete actScene->getChoices()[2];
            choices = std::vector<Choice*>({actScene->getChoices()[0]});
        }
        else if(playerCorruption >= 2)
        {
            delete actScene->getChoices()[0];
            delete actScene->getChoices()[1];
            choices = std::vector<Choice*>({actScene->getChoices()[2]});
        }
        else
        {
            delete actScene->getChoices()[0];
            delete actScene->getChoices()[2];
            choices = std::vector<Choice*>({actScene->getChoices()[1]});
        }
    }

    //adott scene artjának betöltése és renderelése alább
    SDL_Surface* background = SDL_LoadBMP(actScene->getArt().c_str());
    SDL_Surface* dialogFrame = SDL_LoadBMP("assets/dialogframe.bmp");

    SDL_Rect pos_img = {  0,
                          0,
                          background->w, background->h
                       };

    SDL_Rect pos_frame = {  0,
                            420,
                            dialogFrame->w, dialogFrame->h
                         };

    SDL_Color color[2] = {{255,255,255}, {255,0,0}};


    Render::renderSurface(background, pos_img);
    Render::renderSurface(dialogFrame, pos_frame);

    //ha egy adott chapteren belül az első scene-nél tartunk akkor kiírjuk a chapter címét is, kivéve az első chapter esetében ahol
    //az első scene tutorial célokat valósít meg, és ott a másodiknál írjuk ki
    if(actualChapter->getSceneIndex() == 0 && actualChapter->getOrder() > 1)
    {

        SDL_Surface* title = TTF_RenderUTF8_Solid(font, actualChapter->getTitle().c_str(), color[0]);

        SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                              Render::HEIGHT / 4 - title->clip_rect.h,
                              title->w, title->h
                             };

        Render::renderSurface(title, titlePos);

    }
    else if(actualChapter->getOrder() == 1 && actualChapter->getSceneIndex() == 1)
    {
        SDL_Surface* title = TTF_RenderUTF8_Solid(font, actualChapter->getTitle().c_str(), color[0]);

        SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                              Render::HEIGHT / 4 - title->clip_rect.h,
                              title->w, title->h
                             };

        Render::renderSurface(title, titlePos);
    }
    SDL_Surface* story = TTF_RenderUTF8_Solid_Wrapped(storyFont, actualChapter->getActScene()->getStorybit().c_str(), color[0], Render::WIDTH - 50);
    SDL_Rect storyPos =  {Render::WIDTH / 2 - story->clip_rect.w / 2,
                          Render::HEIGHT / 2 - story->clip_rect.h,
                          story->w, story->h
                         };

    Render::renderSurface(story, storyPos);

    SDL_Surface* choiceMenu[choices.size()];
    bool selected[choices.size()];
    SDL_Rect choiceMenuPos[choices.size()];

    //choice-ok kirajzolása dinamikusan egymás alá attól függően egy scene mennyit tartalmaz
    int prevChoiceH = 20;
    for(int choiceIndex = 0; choiceIndex < choices.size(); choiceIndex++)
    {
        std::string text = std::to_string(choiceIndex + 1) + ". " + choices[choiceIndex]->getText();
        //ha nem default típusú choice-ról van szó akkor kiírjuk mennyi szükséges az adott attribútumból a teljesítéshez
        if(choices[choiceIndex]->getType() != static_cast<int>(ChoiceType::Default))
        {
            text += " (" +  std::to_string(choices[choiceIndex]->getDifficulty()) + " " +
                    getChoiceTypeText(static_cast<ChoiceType>(choices[choiceIndex]->getType())) + ")";
        }
        //ha a choice failed állapotú azt is feltüntetjük
        if(choices[choiceIndex]->isFailed())
        {
            text += " [Failed]";
        }

        if(choiceIndex == 0)
        {
            selected[choiceIndex] = true;
            choiceMenu[choiceIndex] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[1], Render::WIDTH - 100);
        }
        else
        {
            selected[choiceIndex] = false;
            choiceMenu[choiceIndex] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[0],  Render::WIDTH - 100);
        }
        choiceMenuPos[choiceIndex] = {Render::WIDTH / 14,
                                      Render::HEIGHT - 220 + prevChoiceH,
                                      choiceMenu[choiceIndex]->w, choiceMenu[choiceIndex]->h
                                     };
        Render::renderSurface(choiceMenu[choiceIndex], choiceMenuPos[choiceIndex]);
        prevChoiceH += (choices[choiceIndex]->getText().length() / 90 * 20) + 30;

    }

    Uint32 time;
    SDL_Event event;
    while(true)
    {
        time=SDL_GetTicks();
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                return -10;
            case SDL_KEYDOWN:
                //enter hatására a kiválaszott choice-t megpróbáljuk teljesíteni, ha sikertelen akkor nem lépünk tovább,
                //egyébként betöltjük a következő scenet vagy annyit ugrunk amennyi az adott choice step értéke volt
                //, viszont ha a chapter utolsó scene-vel volt dolgunk akkor az interludeWindow nyílik meg
                if(SDL_SCANCODE_RETURN == event.key.keysym.scancode)
                {
                    for(int i = 0; i < choices.size(); i++)
                    {
                        if(selected[i] == true)
                        {
                            actScene->chooseChoice(choices[i], player);

                            //secret ending
                            if(actualChapter->getOrder() == 4 && actualChapter->getSceneIndex() == 15) {
                                return -10;
                            }

                            if(choices[i]->isFailed())
                            {
                                return i + 1;
                            }
                            if (choices[i]->getStep() == 0)
                            {
                                //ha lastChapter-nél tartunk akkor visszatérünk a főmenübe
                                if(actualChapter->isLastChapter())
                                {
                                    return -10;
                                }
                                //Random encounter 50-50% eséllyel
                                std::random_device rd;
                                std::mt19937 rng(rd());
                                std::uniform_int_distribution<int> uni(1,10);
                                int random_integer = uni(rng);
                                if(random_integer > 5) {
                                    showEncounter(storyFont, font);
                                }
                                openInterludeWindow(font, storyFont);
                                return 0;
                            }
                            actualChapter->nextScene(choices[i]->getStep());
                            return i + 1;
                        }
                    }
                }
                //fel-le nyilak hatására lehet választani a choiceok kiválasztottságán
                else if(SDLK_UP == event.key.keysym.sym)
                {
                    for(int i = 0; i < choices.size(); i++)
                    {
                        if(selected[i] == true)
                        {
                            selected[i] = false;

                            if(i == 0)
                            {
                                selected[choices.size()-1] = true;
                            }
                            else
                            {
                                selected[i-1] = true;
                            }
                            break;
                        }
                    }
                    for(int i = 0; i < choices.size(); i++)
                    {
                        std::string text = std::to_string(i + 1) + ". " + choices[i]->getText();
                        if(choices[i]->getType() != static_cast<int>(ChoiceType::Default))
                        {
                            text += " (" +  std::to_string(choices[i]->getDifficulty()) + " " +
                                    getChoiceTypeText(static_cast<ChoiceType>(choices[i]->getType())) + ")";
                        }

                        if(choices[i]->isFailed())
                        {
                            text += " [Failed]";
                        }

                        if(selected[i])
                        {
                            choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[1], Render::WIDTH - 100);
                            Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                        }
                        else
                        {
                            choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[0], Render::WIDTH - 100);
                            Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                        }
                    }
                }

                else if(SDLK_DOWN == event.key.keysym.sym)
                {
                    for(int i = 0; i < choices.size(); i++)
                    {
                        if(selected[i] == true)
                        {
                            selected[i] = false;

                            if(i == choices.size()-1)
                            {
                                selected[0] = true;
                            }
                            else
                            {
                                selected[i+1] = true;
                            }
                            break;
                        }
                    }
                    for(int i = 0; i < choices.size(); i++)
                    {
                        std::string text = std::to_string(i + 1) + ". " + choices[i]->getText();
                        if(choices[i]->getType() != static_cast<int>(ChoiceType::Default))
                        {
                            text += " (" +  std::to_string(choices[i]->getDifficulty()) + " " +
                                    getChoiceTypeText(static_cast<ChoiceType>(choices[i]->getType())) + ")";
                        }

                        if(choices[i]->isFailed())
                        {
                            text += " [Failed]";
                        }
                        if(selected[i])
                        {
                            choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[1], Render::WIDTH - 100);
                            Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                        }
                        else
                        {
                            choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[0], Render::WIDTH - 100);
                            Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                        }
                    }
                }
                //j lenyomására megnyílik az attribute window
                else if(SDLK_j == event.key.keysym.sym)
                {
                    openAttribueWindow(storyFont, font);
                    //bezárás esetén kirajzoljuk a játékmenet aktuális állapotát
                    SDL_RenderClear(Render::renderer);
                    background = SDL_LoadBMP(actScene->getArt().c_str());
                    dialogFrame = SDL_LoadBMP("assets/dialogframe.bmp");
                    story = TTF_RenderUTF8_Solid_Wrapped(storyFont, actualChapter->getActScene()->getStorybit().c_str(), color[0], Render::WIDTH - 50);
                    Render::renderSurface(background, pos_img);
                    Render::renderSurface(dialogFrame, pos_frame);
                    if(actualChapter->getSceneIndex() == 0 && actualChapter->getOrder() > 1)
                    {

                        SDL_Surface* title = TTF_RenderUTF8_Solid(font, actualChapter->getTitle().c_str(), color[0]);

                        SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                                              Render::HEIGHT / 4 - title->clip_rect.h,
                                              title->w, title->h
                                             };

                        Render::renderSurface(title, titlePos);

                    }
                    else if(actualChapter->getOrder() == 1 && actualChapter->getSceneIndex() == 1)
                    {
                        SDL_Surface* title = TTF_RenderUTF8_Solid(font, actualChapter->getTitle().c_str(), color[0]);

                        SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                                              Render::HEIGHT / 4 - title->clip_rect.h,
                                              title->w, title->h
                                             };

                        Render::renderSurface(title, titlePos);
                    }

                    Render::renderSurface(story, storyPos);
                    for(int i = 0; i < choices.size(); i++)
                    {
                        std::string text = std::to_string(i + 1) + ". " + choices[i]->getText();
                        if(choices[i]->getType() != static_cast<int>(ChoiceType::Default))
                        {
                            text += " (" +  std::to_string(choices[i]->getDifficulty()) + " " +
                                    getChoiceTypeText(static_cast<ChoiceType>(choices[i]->getType())) + ")";
                        }

                        if(choices[i]->isFailed())
                        {
                            text += " [Failed]";
                        }
                        if(selected[i])
                        {
                            choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[1], Render::WIDTH - 100);
                            Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                        }
                        else
                        {
                            choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[0], Render::WIDTH - 100);
                            Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                        }
                    }
                }
                //i hatására megnyílik az inventory
                else if(SDLK_i == event.key.keysym.sym)
                {
                    openInventory(font);
                    //bezárás után kirajzoljuk a játékmenet aktuális állapotát
                    SDL_RenderClear(Render::renderer);
                    background = SDL_LoadBMP(actScene->getArt().c_str());
                    dialogFrame = SDL_LoadBMP("assets/dialogframe.bmp");
                    story = TTF_RenderUTF8_Solid_Wrapped(storyFont, actualChapter->getActScene()->getStorybit().c_str(), color[0], Render::WIDTH - 50);
                    Render::renderSurface(background, pos_img);
                    Render::renderSurface(dialogFrame, pos_frame);
                    if(actualChapter->getSceneIndex() == 0 && actualChapter->getOrder() > 1)
                    {

                        SDL_Surface* title = TTF_RenderUTF8_Solid(font, actualChapter->getTitle().c_str(), color[0]);

                        SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                                              Render::HEIGHT / 4 - title->clip_rect.h,
                                              title->w, title->h
                                             };

                        Render::renderSurface(title, titlePos);

                    }
                    else if(actualChapter->getOrder() == 1 && actualChapter->getSceneIndex() == 1)
                    {
                        SDL_Surface* title = TTF_RenderUTF8_Solid(font, actualChapter->getTitle().c_str(), color[0]);

                        SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                                              Render::HEIGHT / 4 - title->clip_rect.h,
                                              title->w, title->h
                                             };

                        Render::renderSurface(title, titlePos);
                    }

                    Render::renderSurface(story, storyPos);
                    for(int i = 0; i < choices.size(); i++)
                    {
                        std::string text = std::to_string(i + 1) + ". " + choices[i]->getText();
                        if(choices[i]->getType() != static_cast<int>(ChoiceType::Default))
                        {
                            text += " (" +  std::to_string(choices[i]->getDifficulty()) + " " +
                                    getChoiceTypeText(static_cast<ChoiceType>(choices[i]->getType())) + ")";
                        }

                        if(choices[i]->isFailed())
                        {
                            text += " [Failed]";
                        }
                        if(selected[i])
                        {
                            choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[1], Render::WIDTH - 100);
                            Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                        }
                        else
                        {
                            choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[0], Render::WIDTH - 100);
                            Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                        }
                    }
                }
                //escape hatására megnyithatjuk a játékmenet során elérhető menüt
                else if(SDLK_ESCAPE == event.key.keysym.sym)
                {
                    int result = openInGameMenu(font);
                    SDL_RenderClear(Render::renderer);
                    //bezárás után ha az az értéket kapjuk vissza, amely azt indikálja hogy vissza szeretnénk térni a
                    //főmenübe és félbeszakítani a játékmenetet akkor visszatér a turn
                    if(result == 0)
                    {
                        return -10;
                    }
                    else
                    {
                        background = SDL_LoadBMP(actScene->getArt().c_str());
                        dialogFrame = SDL_LoadBMP("assets/dialogframe.bmp");
                        story = TTF_RenderUTF8_Solid_Wrapped(storyFont, actualChapter->getActScene()->getStorybit().c_str(), color[0], Render::WIDTH - 50);
                        Render::renderSurface(background, pos_img);
                        Render::renderSurface(dialogFrame, pos_frame);
                        if(actualChapter->getSceneIndex() == 0 && actualChapter->getOrder() > 1)
                        {

                            SDL_Surface* title = TTF_RenderUTF8_Solid(font, actualChapter->getTitle().c_str(), color[0]);

                            SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                                                  Render::HEIGHT / 4 - title->clip_rect.h,
                                                  title->w, title->h
                                                 };

                            Render::renderSurface(title, titlePos);

                        }
                        else if(actualChapter->getOrder() == 1 && actualChapter->getSceneIndex() == 1)
                        {
                            SDL_Surface* title = TTF_RenderUTF8_Solid(font, actualChapter->getTitle().c_str(), color[0]);

                            SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                                                  Render::HEIGHT / 4 - title->clip_rect.h,
                                                  title->w, title->h
                                                 };

                            Render::renderSurface(title, titlePos);
                        }

                        Render::renderSurface(story, storyPos);
                        for(int i = 0; i < choices.size(); i++)
                        {
                            std::string text = std::to_string(i + 1) + ". " + choices[i]->getText();
                            if(choices[i]->getType() != static_cast<int>(ChoiceType::Default))
                            {
                                text += " (" +  std::to_string(choices[i]->getDifficulty()) + " " +
                                        getChoiceTypeText(static_cast<ChoiceType>(choices[i]->getType())) + ")";
                            }

                            if(choices[i]->isFailed())
                            {
                                text += " [Failed]";
                            }
                            if(selected[i])
                            {
                                choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[1], Render::WIDTH - 100);
                                Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                            }
                            else
                            {
                                choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, text.c_str(), color[0], Render::WIDTH - 100);
                                Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                            }
                        }
                    }
                }
            }
        }
    }
}

Game::~Game()
{
    delete actualChapter;
    delete player;
}


