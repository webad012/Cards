#include "PlayWindow.hpp"

PlayWindow::PlayWindow()
: currentLevel(1), statusBarWidth(100), movesNum(0), gameDone(false)
{
    changedState = GameWindow::GS_PlayState;
    _statusBar = NULL;
    _gameField = NULL;

    InitializeLevel(currentLevel);
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
    if(gameDone == false)
    {
        if(_gameField->LevelPassed())
        {
            currentLevel++;
            InitializeLevel( currentLevel );
        }

        _statusBar->handle_logic(movesNum);

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

void PlayWindow::InitializeLevel(int levelNum)
{
    std::ifstream levelConfFile;
    levelConfFile.open("levels.conf");
    if( !levelConfFile.is_open() )
    {
        throw std::string(" - PlayWindow::PlayWindow - problem opening levelConfFile");
    }

    int level=-1, num=-1;

    while( (!levelConfFile.eof()) && (level != levelNum) )
    {
        levelConfFile >> level;
        levelConfFile >> num;
    }

    levelConfFile.close();

    if( (level == -1) || (num == -1) )
    {
        throw std::string("error reading level info from file");
    }

    if(_statusBar != NULL)
    {
        delete _statusBar;
        _statusBar = NULL;
    }
    _statusBar = new StatusBar(screen, statusBarWidth, level, movesNum);


    if(_gameField)
    {
        delete _gameField;
        _gameField = NULL;
    }
    _gameField = new GameField(screen, statusBarWidth, num, movesNum);
}

