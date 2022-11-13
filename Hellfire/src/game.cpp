#include "Game.h"
#include "tinyxml2.h"
#include <vector>
#include "choice.h"
#include "scene.h"
#include "render.h"

using namespace tinyxml2;

const char* Game::STORY_PATH = "story/story.xml";
const char* Game::GAME_TITLE = "Fantasy Game";

std::string init = "";
std::vector<Scene> initScns = {};
Chapter Game::actualChapter = Chapter(-1, init, initScns);



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

int Game::openMenu(SDL_Surface* screen, TTF_Font* font, TTF_Font* titleFont) {
    Uint32 time;

    const int NUMMENU=3;
    const char* labels[NUMMENU] = {"Új játék", "Játék betöltése", "Kilépés"};
    SDL_Surface* menus[NUMMENU];
    SDL_Texture* textures[NUMMENU];
    bool selected[NUMMENU] = {0,0,0};
    SDL_Color color[2] = {{255,255,255}, {255,0,0}};

    SDL_SetRenderDrawColor( Render::renderer, 0, 0, 0, 255 );

    SDL_Surface* title = TTF_RenderUTF8_Solid(titleFont, GAME_TITLE, color[0]);

    menus[0] = TTF_RenderUTF8_Solid(font, labels[0], color[0]);
    menus[1] = TTF_RenderUTF8_Solid(font, labels[1], color[0]);
    menus[2] = TTF_RenderUTF8_Solid(font, labels[2], color[0]);
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

    //a menü event ciklusa
    SDL_Event event;
    while(true) {
        time=SDL_GetTicks();
        if(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    return -1;
                case SDL_KEYDOWN:
                    if(SDLK_0 == event.key.keysym.sym) {
                        //ha 0-át nyomunk és nincs kiválasztva:
                        if(!selected[0])
                        {
                            selected[0] = true;

                            if(selected[1]) {
                                selected[1] = false;
                                menus[1] = TTF_RenderUTF8_Solid(font, labels[1], color[0]);
                                Render::renderSurface(menus[1], pos[1]);
                            }
                            else if(selected[2]) {
                                selected[2] = false;
                                menus[2] = TTF_RenderUTF8_Solid(font, labels[2], color[0]);
                                Render::renderSurface(menus[2], pos[2]);
                            }
                            menus[0] = TTF_RenderUTF8_Solid(font, labels[0], color[1]);
                            Render::renderSurface(menus[0], pos[0]);
                        }
                    }

                    //ha valami ki van választva akkor enter lenyomásával visszaadjuk melyik
                     else if(SDL_SCANCODE_RETURN == event.key.keysym.scancode) {
                        for(int i = 0; i < NUMMENU; i++) {
                            if(selected[i] == true) {
                                return i+1;
                            }
                        }
                    }

                     //escape-el kilépünk a menübõl
                    else if(SDLK_ESCAPE == event.key.keysym.sym) {
                        std::cout << "Escape" << std::endl;
                        return 0;
                    }
            }
        }
    }
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


