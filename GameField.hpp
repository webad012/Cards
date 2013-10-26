#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <SDL/SDL.h>
#include <string>
#include <SDL/SDL_gfxPrimitives.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Card.hpp"

class GameField
{
public:
    GameField(SDL_Surface*, int);
    ~GameField();

    void handle_events(SDL_Event);
    void handle_logic(bool*, int*);
    void handle_rendering(bool, SDL_Surface*);

private:
    SDL_Rect fieldRect, fieldBorderRect, restartFontRect;
    SDL_Color ColorBlack, ColorWhite;
    int fieldBorderSize;

    void DrawRectangle(SDL_Rect rect, SDL_Surface *destination, int bodersize, SDL_Color bordercolor);

    std::vector<Card*> cards;
    int numOfCards;
    int numOfFacedUp;
    Uint32 start_time, flip_time;

    int numberOfMoves;

    /// game done part
    SDL_Surface *gameDoneSurface, *restartFontSurface;
    Uint32 gameDoneLastTime, gameDoneBlinkTime;
    bool gameDonePressEnterDraw;
    TTF_Font *restartFont;
};

#endif
