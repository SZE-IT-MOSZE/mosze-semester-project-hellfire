#include <iostream>
#include "game.h"

int main() {

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
