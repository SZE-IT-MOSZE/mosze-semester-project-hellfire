#include "game.h"
#include "tinyxml2.h"
#include <vector>
#include "choice.h"
#include "scene.h"
#include "render.h"
#include "interaction.h"
#include "weapon.h"

using namespace tinyxml2;

const char* Game::STORY_PATH = "story/story.xml";
const char* Game::GAME_TITLE = "Fantasy Game";

Chapter* Game::actualChapter = nullptr;
Weapon init = Weapon();
Player player = Player(init);


Game::Game(){ }

Chapter* Game::getChapter(){
    return actualChapter;
}

Chapter Game::loadNextChapter(){

}

Player& Game::getPlayer(){

}

void Game::loadGame(std::string name){

}

void Game::saveHame(std::string name){

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
    XMLDocument doc;
    doc.LoadFile(STORY_PATH);
    XMLElement * storyRoot = doc.RootElement();
    if (storyRoot != NULL) {
        XMLElement * chapterOne = storyRoot -> FirstChildElement("chapter");
        if(chapterOne != NULL) {
            int cOrder = atoi(chapterOne -> Attribute("order"));
            std::string title = chapterOne -> Attribute("title");
            std::vector<Scene*> scenes = {};
            for (XMLElement* scene = chapterOne->FirstChildElement(); scene != NULL; scene = scene->NextSiblingElement()) {
                     XMLElement * artChild = scene -> FirstChildElement("art");
                     XMLElement * storyChild = scene -> FirstChildElement("storybit");
                     XMLElement * options = scene -> FirstChildElement("options");

                    std::string order = scene -> Attribute("order");
                    std::string art= artChild->GetText();
                    std::string storybit = storyChild -> GetText();
                    std::vector<Choice*> choices = {};
                    for (XMLElement* choice = options->FirstChildElement(); choice != NULL; choice = choice->NextSiblingElement())
                    {
                        int diff = atoi(choice -> Attribute("diff"));
                        int attr = atoi(choice -> Attribute("attr"));
                        int exp = atoi(choice -> Attribute("exp"));
                        int cpn = atoi(choice -> Attribute("cpn"));
                        int step = atoi(choice -> Attribute("step"));
                        std::string txt = choice->GetText();
                        choices.push_back(new Choice(diff, cpn, txt, exp, attr, step));

                    }
                    scenes.push_back(new Scene(storybit, art, order, choices));
            }
           actualChapter = new Chapter(cOrder, title, scenes);
        }
    }
}
int Game::turn(TTF_Font* font, TTF_Font* storyFont){
        if (actualChapter->getSceneIndex() == actualChapter->getSceneCount() - 1) {
            return 0;
        }

        else {
            SDL_RenderClear(Render::renderer);
            SDL_Surface* background = SDL_LoadBMP("assets/Mordor3.bmp");

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

            std::vector<Choice*>& choices =  actualChapter->getActScene()->getChoices();
            for(auto choice : choices) {
                std::cout << choice->getText() << std::endl;
            }
            const char* labels[choices.size()];
            SDL_Surface* choiceMenu[choices.size()];
            bool selected[choices.size()];
            SDL_Rect choiceMenuPos[choices.size()];

            for(int choiceIndex = 0; choiceIndex < choices.size(); choiceIndex++) {
                labels[choiceIndex] = (std::to_string(choiceIndex + 1) + ". " + choices[choiceIndex]->getText()).c_str();
                if(choiceIndex == 0) {
                    selected[choiceIndex] = true;
                    choiceMenu[choiceIndex] = TTF_RenderUTF8_Solid_Wrapped(storyFont, labels[choiceIndex], color[1], Render::WIDTH - 100);
                } else {
                     selected[choiceIndex] = false;
                     choiceMenu[choiceIndex] = TTF_RenderUTF8_Solid_Wrapped(storyFont, labels[choiceIndex], color[0],  Render::WIDTH - 100);
                }
                choiceMenuPos[choiceIndex] = {Render::WIDTH / 14,
                                               Render::HEIGHT - 220 +  choiceMenu[choiceIndex]->clip_rect.h * (choiceIndex + 1),
                                               choiceMenu[choiceIndex]->w, choiceMenu[choiceIndex]->h};
                Render::renderSurface(choiceMenu[choiceIndex], choiceMenuPos[choiceIndex]);
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
                                        actualChapter->nextScene();
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
                                    labels[i] = (std::to_string(i + 1) + ". " + choices[i]->getText()).c_str();
                                    if(selected[i]) {
                                         choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, labels[i], color[1], Render::WIDTH - 100);
                                         Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                                    }
                                    else {
                                      choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, labels[i], color[0], Render::WIDTH - 100);
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
                                    labels[i] = (std::to_string(i + 1) + ". " + choices[i]->getText()).c_str();
                                    if(selected[i]) {
                                         choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, labels[i], color[1], Render::WIDTH - 100);
                                         Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                                    }
                                    else {
                                      choiceMenu[i] = TTF_RenderUTF8_Solid_Wrapped(storyFont, labels[i], color[0], Render::WIDTH - 100);
                                      Render::renderSurface(choiceMenu[i], choiceMenuPos[i]);
                                    }
                               }
                       }
                    }
                }
             }
            // Interaction listener;
           /* int result = listener.listen([&selected, &choices, &choiceMenu, &storyFont, &labels, &color, &choiceMenuPos](SDL_Event& event) -> int {
                    switch(event.type) {
                        case SDL_QUIT:
                            return -1;
                        case SDL_KEYDOWN:
                           if(SDLK_ESCAPE == event.key.keysym.sym) {
                                 return 0;
                           }

                           else if(SDL_SCANCODE_RETURN == event.key.keysym.scancode) {
                                 for(int i = 0; i < choices.size(); i++) {
                                    if(selected[i] == true) {
                                        actualChapter->nextScene();
                                        return i + 1;
                                    }
                                }
                           }

                           else if(SDLK_UP == event.key.keysym.sym) {

                           }

                            else if(SDLK_DOWN == event.key.keysym.sym) {

                           }

                    }
                });
                return result; */
            }
}

Game::~Game() {
    delete actualChapter;
}


