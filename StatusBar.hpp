#ifndef STATUSBAR_HPP
#define STATUSBAR_HPP

#include <iostream>
#include <sstream>

#include "GameWindow.hpp"

class StatusBar
{
public:
    StatusBar(SDL_Surface*, int, int, int);
    ~StatusBar();

    void handle_logic(int);
    void handle_rendering(SDL_Surface*);

    void SetStatusMessage();

private:
    int movesNum;
    SDL_Surface *movesMessageSurface, *movesNumSurface, *levelNumSurface;

    TTF_Font *statusFont;
    SDL_Color textColorBlack;

    SDL_Rect statusRect, movesMessageOffset, movesNumSurfaceOffset, levelNumSurfaceOffset;
};

#endif



