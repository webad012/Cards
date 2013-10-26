#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <SDL/SDL.h>
#include <string>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>

class GameWindow
{
public:
    GameWindow();
    ~GameWindow();

    enum GameState { GS_StartState, GS_ChooseState, GS_PlayState, GS_EndGame };

    virtual void handle_events(bool*) = 0;
    virtual void handle_logic() = 0;
    virtual void handle_rendering() = 0;

    virtual bool ChangedState() = 0;
    virtual GameState GetNewState();

    void ApplySurface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip=NULL);
    void DrawRectangle(SDL_Rect rect, SDL_Surface *destination, int bordersize, SDL_Color bordercolor);
    bool CompareRect(const SDL_Rect, const SDL_Rect);
    static bool IfMouseOverRect(SDL_Event, SDL_Rect);
    static bool IfMouseOverTriangle(SDL_Event, int, int, int, int, int, int);

protected:
    TTF_Font *font50, *font20, *font10;
    SDL_Event event;
    SDL_Color textColorWhite;
    SDL_Surface *screen;

    GameState changedState;
};

#endif
