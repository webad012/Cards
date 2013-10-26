#ifndef CHOOSEWINDOW_HPP
#define CHOOSEWINDOW_HPP

#include <iostream>
#include "GameWindow.hpp"

class ChooseWindow : public GameWindow
{
public:
    ChooseWindow();
    ~ChooseWindow();

    void handle_events(bool*);
    void handle_logic();
    void handle_rendering();

    bool ChangedState();

private:
    SDL_Rect newGameRect, exitGameRect, selectedRect;
    SDL_Surface *newGameSurface, *exitGameSurface;
    SDL_Color borderColorBlue;
};

#endif
