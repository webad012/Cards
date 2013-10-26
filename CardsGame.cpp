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
//    if( _game_state == GameWindow::GS_StartState )
//    {
//        _start_window->handle_logic();
//    }
////    else if( _game_state == GameWindow::GS_ChooseState )
//    {
//        _choose_window->handle_logic();
//    }
//    else if( (_game_state == GameWindow::GS_PlayStatePvP) || (_game_state == GameWindow::GS_PlayStatePvA) )
//    {
//        if(_play_window->GetState() == GameWindow::GS_PlayState)
//        {
//            _play_window->SetState(_game_state);
//
//            if(_game_state == GameWindow::GS_PlayStatePvA)
//            {
//                _play_window->SetAIDifficulty( _choose_window->GetAIDiffictulty() );
//            }
//        }
//
//        _play_window->handle_logic();
//    }
//    else if( _game_state == GameWindow::GS_EndGame )
//    {
//        delete _play_window;
//        _play_window = new PlayWindow();
//        _game_state = GameWindow::GS_ChooseState;
//    }
}

void CardsGame::handle_rendering()
{
    gameWindow->handle_rendering();
//    if( _game_state == GameWindow::GS_StartState )
//    {
//        _start_window->handle_rendering();
//    }
//    else if( _game_state == GameWindow::GS_ChooseState )
//    {
//        _choose_window->handle_rendering();
//    }
//    else if( (_game_state == GameWindow::GS_PlayStatePvP) || (_game_state == GameWindow::GS_PlayStatePvA) )
//    {
//        _play_window->handle_rendering();
//    }
}
