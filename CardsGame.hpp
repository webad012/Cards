#ifndef CARDSGAME_H
#define CARDSGAME_H

#include <SDL/SDL.h>
#include <string>
#include <SDL/SDL_gfxPrimitives.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Card.hpp"

class CardsGame
{
public:
    CardsGame();
    ~CardsGame();

    void handle_events(bool*);
    void handle_logic();
    void handle_rendering();

private:
    SDL_Surface *screen;
    SDL_Event event;

    SDL_Rect fieldBorderRect;
    SDL_Color fieldBorderColor;
    int fieldBorderSize;

    void DrawRectangle(SDL_Rect rect, SDL_Surface *destination, int bodersize, SDL_Color bordercolor);

    std::vector<Card*> cards;
    int numOfCards;
    int numOfFacedUp;
    Uint32 start_time, flip_time;

    int numOfCardsPerRow;
    int numOfCardsPerColumn;
    int numOfHoles;
};

#endif
