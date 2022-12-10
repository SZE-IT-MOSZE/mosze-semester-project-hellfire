#include "game.h"
#include <vector>
#include "choice.h"
#include "scene.h"
#include "render.h"
#include "interaction.h"
#include "weapon.h"
#include "gamestatemanager.h"

const char* Game::GAME_TITLE = "Fantasy Game";

Chapter* Game::actualChapter = nullptr;
Player* Game::player = nullptr;


Game::Game(){ }

Chapter* Game::getChapter(){
    return actualChapter;
}

void Game::loadNextChapter(){

}

Player* Game::getPlayer(){
    return player;
}

void Game::openInventory(){

}

void Game::openAttribueWindow(){

}

int Game::openMenu(SDL_Surface* screen, TTF_Font* font, TTF_Font* titleFont, int previousState) {

    const int NUMMENU=3;
    const char* labels[NUMMENU] = {"Új játék", "Játék betöltése", "Kilépés"};
    SDL_Surface* menus[NUMMENU];
    bool selected[NUMMENU] = {0,0,0};
    selected[previousState] = true;
    SDL_Color color[2] = {{255,255,255}, {255,0,0}};
    SDL_RenderClear(Render::renderer);
    SDL_SetRenderDrawColor( Render::renderer, 0, 0, 0, 255 );
    SDL_Surface* background = SDL_LoadBMP("assets/Mordor1.bmp");
    SDL_Surface* title = TTF_RenderUTF8_Solid(titleFont, GAME_TITLE, color[0]);

    for(int i = 0; i < NUMMENU; i++) {
        if(i == previousState) {
             menus[i] = TTF_RenderUTF8_Solid(font, labels[i], color[1]);
        }
        else {
             menus[i] = TTF_RenderUTF8_Solid(font, labels[i], color[0]);
        }
    }

    SDL_Rect pos[NUMMENU] = { {Render::WIDTH / 2 - menus[0]->clip_rect.w/2,
                              Render::HEIGHT / 2 - menus[0]->clip_rect.h,
                              menus[0]->w, menus[0]->h},
                              {Render::WIDTH / 2 - menus[1]->clip_rect.w/2,
                               Render::HEIGHT / 2 + menus[1]->clip_rect.h,
                               menus[1]->w, menus[1]->h},
                               {Render::WIDTH / 2 - menus[2]->clip_rect.w/2,
                               Render::HEIGHT / 2 + (menus[2]->clip_rect.h * 3),
                               menus[2]->w, menus[2]->h}
                            };

     SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                                   Render::HEIGHT / 4 - title->clip_rect.h,
                                   title->w, title->h};

     SDL_Rect pos_img = {  0,
                0,
                 background->w, background->h};

     Render::renderSurface(background, pos_img);

     Render::renderSurface(title, titlePos);
     Render::renderSurface(menus[0], pos[0]);
     Render::renderSurface(menus[1], pos[1]);
     Render::renderSurface(menus[2], pos[2]);

    Interaction listener;
    int result = listener.listen([&selected, &menus, &font, &labels, &color, &pos](SDL_Event& event) -> int {
            switch(event.type) {
                case SDL_QUIT:
                    return -1;
                case SDL_KEYDOWN:
                   if(SDL_SCANCODE_RETURN == event.key.keysym.scancode) {
                         for(int i = 0; i < NUMMENU; i++) {
                            if(selected[i] == true) {
                                return i;
                            }
                        }
                   }

                   else if(SDLK_UP == event.key.keysym.sym) {
                       for(int i = 0; i < NUMMENU; i++) {
                          if(selected[i] == true) {
                              selected[i] = false;
                              menus[i] = TTF_RenderUTF8_Solid(font, labels[i], color[0]);
                              Render::renderSurface(menus[i], pos[i]);

                              if(i == 0) {
                                 selected[NUMMENU-1] = true;
                                 menus[NUMMENU-1] = TTF_RenderUTF8_Solid(font, labels[NUMMENU-1], color[1]);
                                 Render::renderSurface(menus[NUMMENU-1], pos[NUMMENU-1]);
                              }
                              else {
                                 selected[i-1] = true;
                                 menus[i-1] = TTF_RenderUTF8_Solid(font, labels[i-1], color[1]);
                                 Render::renderSurface(menus[i-1], pos[i-1]);
                              }
                              break;
                          }
                       }
                   }

                    else if(SDLK_DOWN == event.key.keysym.sym) {
                        for(int i = 0; i < NUMMENU; i++) {
                          if(selected[i] == true) {
                              selected[i] = false;
                              menus[i] = TTF_RenderUTF8_Solid(font, labels[i], color[0]);
                              Render::renderSurface(menus[i], pos[i]);

                              if(i == NUMMENU-1) {
                                 selected[0] = true;
                                 menus[0] = TTF_RenderUTF8_Solid(font, labels[0], color[1]);
                                 Render::renderSurface(menus[0], pos[0]);
                              }
                              else {
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

void Game::setChapter(Chapter* newChapter) {
    actualChapter = newChapter;
}

void Game::newGame(){
       GameStateManager stateManager;
       if(actualChapter != nullptr) {
         delete actualChapter;
       }
       if(player != nullptr) {
         delete player;
       }
       actualChapter = stateManager.loadChapterFromXML(2);
       if(actualChapter != nullptr) {
          player = new Player();
          std::cout << "visszatertem" << std::endl;
       } else {
            std::cout << "Failed to load chapter from story.xml." << std::endl;
       }
}
int Game::turn(TTF_Font* font, TTF_Font* storyFont){
        SDL_RenderClear(Render::renderer);

        Scene* actScene = actualChapter->getActScene();
        std::vector<Choice*>& choices =  actScene->getChoices();

        SDL_Surface* background = SDL_LoadBMP(actScene->getArt().c_str());

       SDL_Rect pos_img = {  0,
        0,
         background->w, background->h};

         SDL_Color color[2] = {{255,255,255}, {255,0,0}};


         Render::renderSurface(background, pos_img);

        if(actualChapter->getSceneIndex() == 0) {

            SDL_Surface* title = TTF_RenderUTF8_Solid(font, actualChapter->getTitle().c_str(), color[0]);

            SDL_Rect titlePos =  {Render::WIDTH / 2 - title->clip_rect.w / 2,
                                           Render::HEIGHT / 4 - title->clip_rect.h,
                                           title->w, title->h};

            Render::renderSurface(title, titlePos);

        }
        SDL_Surface* story = TTF_RenderUTF8_Solid_Wrapped(storyFont, actualChapter->getActScene()->getStorybit().c_str(), color[0], Render::WIDTH - 50);
        SDL_Rect storyPos =  {Render::WIDTH / 2 - story->clip_rect.w / 2,
                                          Render::HEIGHT / 2 - story->clip_rect.h,
                                          story->w, story->h};

        Render::renderSurface(story, storyPos);

        SDL_Surface* choiceMenu[choices.size()];
        bool selected[choices.size()];
        SDL_Rect choiceMenuPos[choices.size()];

        int prevChoiceH = 30;
        for(int choiceIndex = 0; choiceIndex < choices.size(); choiceIndex++) {
            if(choiceIndex == 0) {
                selected[choiceIndex] = true;
                choiceMenu[choiceIndex] = TTF_RenderUTF8_Solid_Wrapped(storyFont, (std::to_string(choiceIndex + 1) + ". " + choices[choiceIndex]->getText()).c_str(), color[1], Render::WIDTH - 100);
            } else {
                 selected[choiceIndex] = false;
                 choiceMenu[choiceIndex] = TTF_RenderUTF8_Solid_Wrapped(storyFont, (std::to_string(choiceIndex + 1) + ". " + choices[choiceIndex]->getText()).c_str(), color[0],  Render::WIDTH - 100);
            }
            choiceMenuPos[choiceIndex] = {Render::WIDTH / 14,
                                           Render::HEIGHT - 220 + prevChoiceH,
                                           choiceMenu[choiceIndex]->w, choiceMenu[choiceIndex]->h};
            Render::renderSurface(choiceMenu[choiceIndex], choiceMenuPos[choiceIndex]);
            prevChoiceH += (choices[choiceIndex]->getText().length() / 100 * 20) + 30;

        }

         Uint32 time;
         SDL_Event event;
         while(true) {
            time=SDL_GetTicks();
            if(SDL_PollEvent(&event)) {
                 switch(event.type) {
                    case SDL_QUIT:
                        return -1;
                    case SDL_KEYDOWN:
                       if(SDLK_ESCAPE == event.key.keysym.sym) {
                            // return 0;
                       }

                       else if(SDL_SCANCODE_RETURN == event.key.keysym.scancode) {
                             for(int i = 0; i < choices.size(); i++) {
                                if(selected[i] == true) {
                                    actScene->chooseChoice(choices[i], player);
                                    if (choices[i]->getStep() == 0) {
                                        return 0;
                                    }
                                    actualChapter->nextScene(choices[i]->getStep());
                                    return i + 1;
                                }
                            }
                       }

                    else if(SDLK_UP == event.key.keysym.sym) {
                       for(int i = 0; i < choices.size(); i++) {
                              if(selected[i] == true) {
                                  selected[i] = false;

                                  if(i == 0) {
                                     selected[choices.size()-1] = true;
                                  }
                                  else {
                                     selected[i-1] = true;
                                  }
                                  break;
                              }
                           }
                           for(int i = 0; i < choices.size(); i++) {

                                if(selected[i]) {
                                     choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, (std::to_string(i + 1) + ". " + choices[i]->getText()).c_str(), color[1], Render::WIDTH - 100);
                                     Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                                }
                                else {
                                  choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, (std::to_string(i + 1) + ". " + choices[i]->getText()).c_str(), color[0], Render::WIDTH - 100);
                                  Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                                }
                           }
                    }

                    else if(SDLK_DOWN == event.key.keysym.sym) {
                         for(int i = 0; i < choices.size(); i++) {
                              if(selected[i] == true) {
                                  selected[i] = false;

                                  if(i == choices.size()-1) {
                                     selected[0] = true;
                                  }
                                  else {
                                     selected[i+1] = true;
                                  }
                                  break;
                              }
                           }
                           for(int i = 0; i < choices.size(); i++) {

                                if(selected[i]) {
                                     choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, (std::to_string(i + 1) + ". " + choices[i]->getText()).c_str(), color[1], Render::WIDTH - 100);
                                     Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                                }
                                else {
                                  choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, (std::to_string(i + 1) + ". " + choices[i]->getText()).c_str(), color[0], Render::WIDTH - 100);
                                  Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                                }
                           }
                   }
                }
            }
         }
}

Game::~Game() {
    delete actualChapter;
    delete player;
}


