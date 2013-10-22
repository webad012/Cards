#ifndef CARDS_H
#define CARDS_H

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <sstream>
#include <iostream>

class Card
{
public:
    Card(int, SDL_Rect, int, int);
    ~Card();

//    void handle_events(SDL_Event);
    void handle_logic();
    void handle_rendering(SDL_Surface*);

    void HandleLeftMouseClick(SDL_Event);
    void HandleRightMouseClick(SDL_Event);

    void Swap(Card*);

    void SetUnchosen();

    int GetNum();
    void SetFacedDown();
    int GetCoordI();
    int GetCoordJ();
    SDL_Rect GetRect();

    bool GetFacedUp();
    bool GetChosen();

    bool CloseForAction(Card*);

    bool IsHole();
    void SetHole();

    void SetCoordI(int);
    void SetCoordJ(int);
    void SetRect(SDL_Rect);

    void Remove();
    bool IsRemoved();

private:
    SDL_Rect cardRect;
    int cardNum;
    bool facedUp, chosen, removed, hole;

    SDL_Surface *cardNumSurface;
    SDL_Color colorBlack;
    TTF_Font *cardNumFont;
    int coordi, coordj;
};

#endif
