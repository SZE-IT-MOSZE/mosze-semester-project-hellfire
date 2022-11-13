#include <iostream>
#include "game.h"
#include "render.h"


int main(int argc, char *argv[]) {
    SDL_Surface* windowSurface = SDL_GetWindowSurface(Render::window);
    SDL_Surface* imageSurface = SDL_LoadBMP("assets/Mordor1.bmp");

    SDL_Rect pos_img = {  0,
                0,
                 imageSurface->w, imageSurface->h};

    Render::renderSurface(imageSurface, pos_img);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("assets/arial.ttf", 30);
    TTF_Font *titleFont = TTF_OpenFont("assets/hobbiton.ttf", 50);

    int result = 0;
    while(true) {
        result = Game::openMenu(windowSurface, font, titleFont, result);
        if(result == 2) {
            break;
        }
        else if(result == 0) {
           if(Game::getChapter().getOrder() == -1) {
               Game::newGame();
               std::cout << "Chapter title: " << Game::getChapter().getTitle() << std::endl;
               std::cout << "Chapter order: " << Game::getChapter().getOrder() << std::endl;
               for (Scene* scene = Game::getChapter().nextScene(); scene != NULL; scene = Game::getChapter().nextScene()) {
                     std::cout << "-------------------------------" << std::endl;
                     std::cout << "Scene order number: " << scene->getOrder() << std::endl;
                     std::cout << "Art path: " << scene->getArt() << std::endl;
                     std::cout << "Storybit: " << scene->getStorybit() << std::endl;
                     std::cout << "Choices: " << std::endl;
                     for (auto & choice : scene->getChoices()) {
                        std::cout << "\tChoice difficulty: " << choice.getDifficulty() << std::endl;
                        std::cout << "\tChoice experience: " << choice.getExperience() << std::endl;
                        std::cout << "\tChoice corruption: " << choice.getCorruption() << std::endl;
                        std::cout << "\tChoice text: " << choice.getText() << std::endl << std::endl;
                    }
                     std::cout << "-------------------------------" << std::endl;
                }
           }
        }
    }


    SDL_FreeSurface(windowSurface);
    SDL_DestroyWindow(Render::window);
    SDL_DestroyRenderer( Render::renderer );
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_CloseFont(titleFont);
    TTF_Quit();
    return EXIT_SUCCESS;
}
