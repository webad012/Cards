#include <cstddef>
#include <string>
#include <SDL/SDL.h>
#include <iostream>

#include "CardsGame.hpp"

using namespace std;

int main()
{
    try
    {
        bool quit=false;
        CardsGame *game = NULL;
        game = new CardsGame();

        if(game == NULL)
        {
            throw std::string("creating game problem");
        }

        while(quit == false)
        {
            /// events
            game->handle_events(&quit);

            /// logic
            game->handle_logic();

            /// rendering
            game->handle_rendering();

            SDL_Delay(5);
        }

        delete game;
        game = NULL;

        return 0;
    }
    catch(std::string s)
    {
        std::cout << "STRING EXCEPTION - main - " << s << std::endl;;
    }
    catch(std::exception &e)
    {
        std::cout << "UNHANDLED EXCEPTION - main - " << e.what() << std::endl;
    }

    return 1;
}
