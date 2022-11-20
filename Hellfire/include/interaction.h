#ifndef INTERACTION_H
#define INTERACTION_H

#include <iostream>
#include <functional>
#include <SDL2/SDL.h>

class Interaction
{
    public:
         Interaction();
         int listen(std::function<int(SDL_Event&)> callback);
    protected:

    private:
         SDL_Event event;
};

#endif

