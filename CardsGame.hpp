#ifndef CARDSGAME_H
#define CARDSGAME_H

#include <SDL/SDL.h>
#include <string>
#include <SDL/SDL_ttf.h>

#include "GameWindow.hpp"
#include "StartWindow.hpp"
#include "ChooseWindow.hpp"
#include "PlayWindow.hpp"

class CardsGame
{
public:
    CardsGame();
    ~CardsGame();

    void handle_events(bool*);
    void handle_logic();
    void handle_rendering();

private:
//    GameWindow::GameState _game_state;

    GameWindow *gameWindow;

//    StartWindow *_start_window;
//    ChooseWindow *_choose_window;
};
#endif
