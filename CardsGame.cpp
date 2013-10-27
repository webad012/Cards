#include "CardsGame.hpp"

CardsGame::CardsGame()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        throw std::string("init problem");
    }

    if( TTF_Init() == -1 )
    {
        throw std::string( "font init problem" );
    }

    SDL_WM_SetCaption( "Cards", NULL );

    gameWindow = new StartWindow();
}

CardsGame::~CardsGame()
{
    delete gameWindow;
    gameWindow = NULL;
    TTF_Quit();
    SDL_Quit();
}

void CardsGame::handle_events(bool *quit)
{
    gameWindow->handle_events(quit);
}

void CardsGame::handle_logic()
{
    if(gameWindow->ChangedState())
    {
        switch(gameWindow->GetNewState())
        {
            case GameWindow::GS_StartState:
                delete gameWindow;
                gameWindow = new StartWindow();
                break;
            case GameWindow::GS_ChooseState:
                delete gameWindow;
                gameWindow = new ChooseWindow();
                break;
            case GameWindow::GS_PlayState:
                delete gameWindow;
                gameWindow = new PlayWindow();
                break;
            case GameWindow::GS_EndGame:
                delete gameWindow;
                gameWindow = new ChooseWindow();
                break;
        }
    }

    gameWindow->handle_logic();
}

void CardsGame::handle_rendering()
{
    gameWindow->handle_rendering();
}
