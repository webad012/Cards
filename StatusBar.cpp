#include "StatusBar.hpp"

StatusBar::StatusBar(SDL_Surface* screen, int width, int levelNum, int movenum)
: movesNum(movenum)
{
    statusRect.x = screen->w - width;
    statusRect.y = 0;
    statusRect.w = width;
    statusRect.h = screen->h;

    statusFont = TTF_OpenFont( "CardsFont.ttf", 20 );
    if( statusFont == NULL )
    {
        throw std::string(" - StatusBar::StatusBar - statusFont problem - " + std::string(TTF_GetError()));
    }

    textColorBlack.r = 0;
    textColorBlack.g = 0;
    textColorBlack.b = 0;
    textColorBlack.unused = 0;

    std::stringstream messageSS;

    messageSS << "Nivo: " << levelNum;
    levelNumSurface = TTF_RenderUTF8_Blended(statusFont, messageSS.str().c_str(), textColorBlack);
    levelNumSurfaceOffset.x = statusRect.x+5;
    levelNumSurfaceOffset.y = statusRect.y + 20;

    movesMessageSurface = TTF_RenderUTF8_Blended(statusFont, "Br. poteza", textColorBlack);
    movesMessageOffset.x = levelNumSurfaceOffset.x;
    movesMessageOffset.y = levelNumSurfaceOffset.y + levelNumSurface->h + statusRect.h/4;

    messageSS.str("");
    messageSS.clear();
    messageSS << movesNum;
    movesNumSurface = TTF_RenderUTF8_Blended(statusFont, messageSS.str().c_str(), textColorBlack);

    movesNumSurfaceOffset.x = movesMessageOffset.x;
    movesNumSurfaceOffset.y = movesMessageOffset.y + movesMessageSurface->h;
}

StatusBar::~StatusBar()
{
    TTF_CloseFont(statusFont);
    SDL_FreeSurface(movesMessageSurface);
    SDL_FreeSurface(movesNumSurface);
}

void StatusBar::handle_logic(int num)
{
    if( num != movesNum )
    {
        movesNum = num;
        std::stringstream messageSS;
        messageSS << movesNum;

        SDL_FreeSurface(movesNumSurface);
        movesNumSurface = TTF_RenderUTF8_Solid(statusFont, messageSS.str().c_str(), textColorBlack);
    }
}

void StatusBar::handle_rendering(SDL_Surface *screen)
{
    SDL_FillRect(screen, &statusRect, 0x888888);

    SDL_BlitSurface(levelNumSurface, NULL, screen, &levelNumSurfaceOffset);
    SDL_BlitSurface(movesMessageSurface, NULL, screen, &movesMessageOffset);
    SDL_BlitSurface(movesNumSurface, NULL, screen, &movesNumSurfaceOffset);

}
