#include "interaction.h"

Interaction::Interaction()
{

}

//higher order metódus, mely egy callbackot burkol SDL event események lekezelésére
int Interaction::listen(std::function<int(SDL_Event&)> callback)
{
    Uint32 time;
    int result = -1;
    while(true)
    {
        time=SDL_GetTicks();
        if(SDL_PollEvent(&event))
        {
            result = callback(event);
            if(result > -1 && result < 5)
            {
                break;
            }
        }
    }
    return result;
}
