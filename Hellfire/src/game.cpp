#include "game.h"
#include <vector>
#include "choice.h"
#include "scene.h"
#include "render.h"
#include "interaction.h"
#include "weapon.h"
#include "gamestatemanager.h"

const char* Game::GAME_TITLE = "MOrdor'S Zealous Enmity: A Lord of the Rings story";

Chapter* Game::actualChapter = nullptr;
Player* Game::player = nullptr;


Game::Game() { }

void Game::saveGame() {
    GameStateManager stateManager;
    stateManager.saveGameStateToXML(player, actualChapter->getOrder(), actualChapter->getSceneIndex(), &actualChapter->getActScene()->getChoices());
}

bool Game::loadGame() {
    GameStateManager stateManager;
    bool result = stateManager.loadGameStateFromXML(player, actualChapter);
    if(result) {
        player = stateManager.getPlayerState();
        actualChapter = stateManager.getChapterState();
    }
    return result;
}

Chapter* Game::getChapter()
{
    return actualChapter;
}

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

int Game::openInGameMenu(TTF_Font* font)
{
    SDL_RenderClear(Render::renderer);
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
                         title->w, title->h};

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
                if(SDL_SCANCODE_RETURN == event.key.keysym.scancode)
                {
                    if(selected[1])
                    {
                        return 0;
                    }
                    else
                    {
                        //Mentés
                        saveGame();
                        return 1;
                    }
                }
                else if(SDLK_ESCAPE == event.key.keysym.sym)
                {
                    return 1;
                }
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

void Game::openInventory()
{

}

void Game::openAttribueWindow(TTF_Font* font, TTF_Font* titleFont)
{
    SDL_RenderClear(Render::renderer);
    SDL_Color color[2] = {{255,255,255}, {255,0,0}};
    bool selected[3] = {true, false, false};
    Attributes* playerAttributes = player->getAttributes();

    std::string skillPointsText = "Szabad skill pontok: " + std::to_string(player->getSkillPoints());
    SDL_Surface* skillPoints = TTF_RenderUTF8_Solid(titleFont, skillPointsText.c_str(), color[0]);

    std::string strengthText = "Strength: " + std::to_string(playerAttributes->getStrength());
    std::string intelligenceText = "Intelligence: " + std::to_string(playerAttributes->getIntelligence());
    std::string persuasionText = "Persuasion: " + std::to_string(playerAttributes->getPersuasion());

    SDL_Surface* strength = TTF_RenderUTF8_Solid(font, strengthText.c_str(), color[0]);
    SDL_Surface* intelligence = TTF_RenderUTF8_Solid(font, intelligenceText.c_str(), color[0]);
    SDL_Surface* persuasion = TTF_RenderUTF8_Solid(font, persuasionText.c_str(), color[0]);

    SDL_Surface* toggles[3] =
    {
        TTF_RenderUTF8_Solid(font, "Hozzáad", color[1]),
        TTF_RenderUTF8_Solid(font, "Hozzáad", color[0]),
        TTF_RenderUTF8_Solid(font, "Hozzáad", color[0]),
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
                    skillPointsText = "Szabad skill pontok: " + std::to_string(player->getSkillPoints());
                    skillPoints = TTF_RenderUTF8_Solid(titleFont, skillPointsText.c_str(), color[0]);

                    strengthText = "Strength: " + std::to_string(playerAttributes->getStrength());
                    intelligenceText = "Intelligence: " + std::to_string(playerAttributes->getIntelligence());
                    persuasionText = "Persuasion: " + std::to_string(playerAttributes->getPersuasion());

                    strength = TTF_RenderUTF8_Solid(font, strengthText.c_str(), color[0]);
                    intelligence = TTF_RenderUTF8_Solid(font, intelligenceText.c_str(), color[0]);
                    persuasion = TTF_RenderUTF8_Solid(font, persuasionText.c_str(), color[0]);

                    Render::renderSurface(skillPoints, titlePos);
                    Render::renderSurface(strength, pos[0]);
                    Render::renderSurface(intelligence, pos[1]);
                    Render::renderSurface(persuasion, pos[2]);
                }
                else if(SDLK_j == event.key.keysym.sym)
                {
                    return;
                }
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
                }
            }
        }
    }
}

int Game::openMenu(SDL_Surface* screen, TTF_Font* font, TTF_Font* titleFont, int previousState)
{

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
            return -1;
        case SDL_KEYDOWN:
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

            else if(SDLK_UP == event.key.keysym.sym)
            {
                for(int i = 0; i < NUMMENU; i++)
                {
                    if(selected[i] == true)
                    {
                        selected[i] = false;
                        menus[i] = TTF_RenderUTF8_Solid(font, labels[i], color[0]);
                        Render::renderSurface(menus[i], pos[i]);

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

void Game::openInterludeWindow(TTF_Font* titleFont, TTF_Font* font)
{
    SDL_RenderClear(Render::renderer);
    SDL_Color color[2] = {{255,255,255}, {255,0,0}};
    SDL_Surface* title = TTF_RenderUTF8_Solid(titleFont, "Fejezet vége", color[0]);

    int playerExperience = player->getExperience();
    std::string experienceBeforeConvertText = "Experience pontok átváltás előtt: " + std::to_string(playerExperience) + " exp";
    std::string experienceAfterConvertText = "Experience pontok átváltás után: " + std::to_string(playerExperience - (playerExperience / 100 * 100)) + " exp";
    std::string gainedSkillpointsText = "Szerzett skill pontok: " + std::to_string(playerExperience / 100);

    SDL_Surface* experienceBeforeConvert = TTF_RenderUTF8_Solid(font, experienceBeforeConvertText.c_str(), color[0]);
    SDL_Surface* experienceAfterConvert = TTF_RenderUTF8_Solid(font, experienceAfterConvertText.c_str(), color[0]);
    SDL_Surface* gainedSkillPoints = TTF_RenderUTF8_Solid(font, gainedSkillpointsText.c_str(), color[0]);

    SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                          Render::HEIGHT / 4 - title->clip_rect.h,
                          title->w, title->h
                         };
    SDL_Rect pos[3] =  { {
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
        }
    };


    Render::renderSurface(title, titlePos);
    Render::renderSurface(experienceBeforeConvert, pos[0]);
    Render::renderSurface(experienceAfterConvert, pos[1]);
    Render::renderSurface(gainedSkillPoints, pos[2]);


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
                if(SDL_SCANCODE_RETURN == event.key.keysym.scancode)
                {
                    player->setExperience(-(playerExperience / 100 * 100));
                    player->setSkillPoints(playerExperience / 100);
                    int prevChapterOrder = actualChapter->getOrder();
                    loadNextChapter(prevChapterOrder + 1);
                    return;
                }
            }
        }
    }
}


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
int Game::turn(TTF_Font* font, TTF_Font* storyFont)
{
    SDL_RenderClear(Render::renderer);

    Scene* actScene = actualChapter->getActScene();
    std::vector<Choice*>& choices =  actScene->getChoices();

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

    int prevChoiceH = 20;
    for(int choiceIndex = 0; choiceIndex < choices.size(); choiceIndex++)
    {
        std::string text = std::to_string(choiceIndex + 1) + ". " + choices[choiceIndex]->getText();
        if(choices[choiceIndex]->getType() != static_cast<int>(ChoiceType::Default))
        {
            text += " (" +  std::to_string(choices[choiceIndex]->getDifficulty()) + " " +
                    getChoiceTypeText(static_cast<ChoiceType>(choices[choiceIndex]->getType())) + ")";
        }
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
                return -1;
            case SDL_KEYDOWN:
                if(SDL_SCANCODE_RETURN == event.key.keysym.scancode)
                {
                    for(int i = 0; i < choices.size(); i++)
                    {
                        if(selected[i] == true)
                        {
                            actScene->chooseChoice(choices[i], player);
                            if(choices[i]->isFailed())
                            {
                                return i + 1;
                            }
                            if (choices[i]->getStep() == 0)
                            {
                                openInterludeWindow(font, storyFont);
                                return 0;
                            }
                            actualChapter->nextScene(choices[i]->getStep());
                            return i + 1;
                        }
                    }
                }

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
                else if(SDLK_j == event.key.keysym.sym)
                {
                    openAttribueWindow(storyFont, font);
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
                else if(SDLK_ESCAPE == event.key.keysym.sym)
                {
                    int result = openInGameMenu(font);
                    SDL_RenderClear(Render::renderer);
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


