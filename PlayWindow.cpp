#include "PlayWindow.hpp"

PlayWindow::PlayWindow()
: statusBarHeight(30), movesNum(0)
{
    changedState = GameWindow::GS_PlayState;
    _statusBar = new StatusBar(screen, statusBarHeight);
    _gameField = new GameField(screen, statusBarHeight);
    gameDone = false;
}

PlayWindow::~PlayWindow()
{
    delete _statusBar;
    delete _gameField;
}

void PlayWindow::handle_events(bool* quit)
{
    while( SDL_PollEvent( &event ) )
    {
        if(gameDone == false)
        {
            _gameField->handle_events(event);
        }
        else if(gameDone == true)
        {
            if( (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_RETURN) )
            {
                changedState = GameWindow::GS_EndGame;
            }
        }

        if( event.type == SDL_QUIT )
        {
            *quit = true;
        }
    }
}

void PlayWindow::handle_logic()
{
    if(gameDone == true)
    {
         _statusBar->handle_logic(true, movesNum);
    }
    else
    {
        _statusBar->handle_logic(false, movesNum);
        _gameField->handle_logic(&gameDone, &movesNum);
    }
}

void PlayWindow::handle_rendering()
{
    SDL_FillRect(screen, NULL, 0x000000);
    _statusBar->handle_rendering(screen);
    _gameField->handle_rendering(gameDone, screen);

    if( SDL_Flip(screen) == -1 )
    {
        throw std::string("flip problem");
    }
}

bool PlayWindow::ChangedState()
{
    return changedState != GameWindow::GS_PlayState;
}

