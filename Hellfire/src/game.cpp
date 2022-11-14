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

std::vector<Scene> initScns = {};
Chapter Game::actualChapter = Chapter(initScns);
Weapon init = Weapon();
Player player = Player(init);


Game::Game(){ }

Chapter& Game::getChapter(){
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
    SDL_Texture* textures[NUMMENU];
    bool selected[NUMMENU] = {0,0,0};
    selected[previousState] = true;
    SDL_Color color[2] = {{255,255,255}, {255,0,0}};

    SDL_SetRenderDrawColor( Render::renderer, 0, 0, 0, 255 );

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


void Game::newGame(){
    XMLDocument doc;
    doc.LoadFile(STORY_PATH);
    XMLElement * storyRoot = doc.RootElement();
    if (storyRoot != NULL) {
        XMLElement * chapterOne = storyRoot -> FirstChildElement("chapter");
        if(chapterOne != NULL) {
            int cOrder = atoi(chapterOne -> Attribute("order"));
            std::string title = chapterOne -> Attribute("title");
            std::vector<Scene> scenes = {};
            for (XMLElement* scene = chapterOne->FirstChildElement(); scene != NULL; scene = scene->NextSiblingElement()) {
                     XMLElement * artChild = scene -> FirstChildElement("art");
                     XMLElement * storyChild = scene -> FirstChildElement("storybit");
                     XMLElement * options = scene -> FirstChildElement("options");

                    std::string order = scene -> Attribute("order");
                    std::string art= artChild->GetText();
                    std::string storybit = storyChild -> GetText();
                    std::vector<Choice> choices = {};
                    for (XMLElement* choice = options->FirstChildElement(); choice != NULL; choice = choice->NextSiblingElement())
                    {
                        int diff = atoi(choice -> Attribute("diff"));
                        int attr = atoi(choice -> Attribute("attr"));
                        int exp = atoi(choice -> Attribute("exp"));
                        int cpn = atoi(choice -> Attribute("cpn"));
                        int step = atoi(choice -> Attribute("step"));
                        std::string txt = choice->GetText();
                        choices.push_back(Choice(diff, cpn, txt, exp, attr, step));

                    }
                    scenes.push_back(Scene(storybit, art, order, choices));
            }
            Chapter act = Chapter(cOrder, title, scenes);
            actualChapter = act;
        }
    }
}
void Game::turn(){

}


