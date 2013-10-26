#include "StatusBar.hpp"

StatusBar::StatusBar(SDL_Surface* screen, int height)
{
    statusRect.x = 0;
    statusRect.y = 0;
    statusRect.w = screen->w;
    statusRect.h = height;

    statusFont = TTF_OpenFont( "CardsFont.ttf", 20 );
    if( statusFont == NULL )
    {
        throw std::string(" - StatusBar::StatusBar - statusFont problem - " + std::string(TTF_GetError()));
    }

    textColorBlack.r = 0;
    textColorBlack.g = 0;
    textColorBlack.b = 0;
    textColorBlack.unused = 0;

    movesNum = 0;

    messageSS << "Broj poteza: " << movesNum;
    messageSurface = TTF_RenderUTF8_Blended(statusFont, messageSS.str().c_str(), textColorBlack);
    messageSurfaceOffset.x = (statusRect.w - messageSurface->w)/2;
    messageSurfaceOffset.y = (statusRect.h - messageSurface->h)/2;

    previous_gamedone_status = true;
}

StatusBar::~StatusBar()
{
    TTF_CloseFont(statusFont);
    SDL_FreeSurface(messageSurface);
}

void StatusBar::handle_logic(bool gameDone, int num)
{
    if((gameDone == false) && (num != movesNum))
    {
        movesNum = num;
        messageSS.str("");
        messageSS.clear();
        messageSS << "Broj poteza: " << movesNum;

        SDL_FreeSurface(messageSurface);
        messageSurface = TTF_RenderUTF8_Solid(statusFont, messageSS.str().c_str(), textColorBlack);
        previous_gamedone_status = false;
    }
    else if((gameDone == true) && (previous_gamedone_status == false))
    {
        messageSS.str("");
        messageSS.clear();
        messageSS << "Igra završena sa " << num << " poteza.";

        SDL_FreeSurface(messageSurface);
        messageSurface = TTF_RenderUTF8_Solid(statusFont, messageSS.str().c_str(), textColorBlack);
        previous_gamedone_status = true;
    }
}

void StatusBar::handle_rendering(SDL_Surface *screen)
{
    SDL_FillRect(screen, &statusRect, 0x888888);

    SDL_BlitSurface(messageSurface, NULL, screen, &messageSurfaceOffset);

}
