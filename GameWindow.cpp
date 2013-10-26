
#include "GameWindow.hpp"

GameWindow::GameWindow()
{
    screen = SDL_SetVideoMode(600, 630, 32, SDL_SWSURFACE );
    if( screen == NULL )
    {
        throw std::string(" - GameWindow::GameWindow() - screen init problem");;
    }

    font50 = TTF_OpenFont( "CardsFont.ttf", 50 );
    if( font50 == NULL )
    {
        throw std::string(" - GameWindow::GameWindow()font50 problem - " + std::string(TTF_GetError()));
    }

    font20 = TTF_OpenFont( "CardsFont.ttf", 20 );
    if( font20 == NULL )
    {
        throw std::string(" - GameWindow::GameWindow()font20 problem - " + std::string(TTF_GetError()));
    }

    font10 = TTF_OpenFont( "CardsFont.ttf", 10 );
    if( font10 == NULL )
    {
        throw std::string(" - GameWindow::GameWindow()font10 problem - " + std::string(TTF_GetError()));
    }

    textColorWhite.r = 255;
    textColorWhite.g = 255;
    textColorWhite.b = 255;
}

GameWindow::~GameWindow()
{
    TTF_CloseFont(font10);
    TTF_CloseFont(font20);
    TTF_CloseFont(font50);
}

void GameWindow::ApplySurface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip)
{
    SDL_Rect offset;
    offset.x=x;
    offset.y=y;

    SDL_BlitSurface(source, clip, destination, &offset);
}

void GameWindow::DrawRectangle(SDL_Rect rect, SDL_Surface *destination, int bodersize, SDL_Color bordercolor)
{
    int bordX1 = rect.x - 5;
    int bordY1 = rect.y - 5;
    int bordX2 = rect.x + rect.w + 5;
    int bordY2 = rect.y + rect.h + 5;

    for(int i=0; i < bodersize; i++)
    {
        rectangleRGBA(destination, bordX1-i, bordY1-i, bordX2+i, bordY2+i, bordercolor.r, bordercolor.g, bordercolor.b, bordercolor.unused);
    }
}

bool GameWindow::CompareRect(const SDL_Rect r1, const SDL_Rect r2)
{
    if((r1.x==r2.x) && (r1.y==r2.y) && (r1.w==r2.w) && (r1.h==r2.h))
    {
        return true;
    }

    return false;
}

bool GameWindow::IfMouseOverRect(SDL_Event e, SDL_Rect r)
{
    int evtx = e.button.x;
    int evty = e.button.y;

    if( ( evtx > r.x ) && ( evtx< r.x + r.w ) && ( evty > r.y ) && ( evty < r.y + r.h ) )
    {
        return true;
    }

    return false;
}

bool GameWindow::IfMouseOverTriangle(SDL_Event e, int x1, int y1, int x2, int y2, int x3, int y3)
{
    bool b1, b2, b3;

    b1 = ((e.button.x - x2) * (y1 - y2)) - ((x1 - x2) * (e.button.y - y2)) < 0.0f;
    b2 = (e.button.x - x3) * (y2- y3) - (x2 - x3) * (e.button.y - y3) < 0.0f;
    b3 = (e.button.x - x1) * (y3 - y1) - (x3 - x1) * (e.button.y - y1) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

GameWindow::GameState GameWindow::GetNewState()
{
    return changedState;
}
