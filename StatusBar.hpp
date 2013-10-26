#ifndef STATUSBAR_HPP
#define STATUSBAR_HPP

#include <iostream>
#include <sstream>

#include "GameWindow.hpp"

class StatusBar
{
public:
    StatusBar(SDL_Surface*, int height);
    ~StatusBar();

    void handle_logic(bool, int);
    void handle_rendering(SDL_Surface*);

    void SetStatusMessage();

private:
    int movesNum;
    SDL_Surface *messageSurface;
    bool previous_gamedone_status;

    TTF_Font *statusFont;
    SDL_Color textColorBlack;

    SDL_Rect statusRect, messageSurfaceOffset;

    std::stringstream messageSS;
};

#endif



