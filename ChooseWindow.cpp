#include "ChooseWindow.hpp"

ChooseWindow::ChooseWindow()
{
    changedState = GameWindow::GS_ChooseState;

    newGameSurface = TTF_RenderUTF8_Blended(font20, "Nova igra", textColorWhite);
    exitGameSurface = TTF_RenderUTF8_Blended(font20, "Izađi", textColorWhite);

    newGameRect.x = (screen->w - newGameSurface->w)/2;
    newGameRect.y = screen->h/4;
    newGameRect.h = newGameSurface->h;
    newGameRect.w = newGameSurface->w;

    exitGameRect.x = (screen->w - exitGameSurface->w)/2;
    exitGameRect.y = screen->h - screen->h/4;
    exitGameRect.w = exitGameSurface->w;
    exitGameRect.h = exitGameSurface->h;

    borderColorBlue.r = 50;
    borderColorBlue.g = 50;
    borderColorBlue.b = 150;
    borderColorBlue.unused = 255;

    selectedRect = newGameRect;
}

ChooseWindow::~ChooseWindow()
{
    SDL_FreeSurface(newGameSurface);
    SDL_FreeSurface(exitGameSurface);
}

void ChooseWindow::handle_events(bool* quit)
{
    while( SDL_PollEvent( &event ) )
    {
        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_UP)
            {
                if( CompareRect(selectedRect, newGameRect) )
                {
                    selectedRect = exitGameRect;
                }
                else if( CompareRect(selectedRect, exitGameRect) )
                {
                    selectedRect = newGameRect;
                }
            }
            else if(event.key.keysym.sym == SDLK_DOWN)
            {
                if( CompareRect(selectedRect, newGameRect) )
                {
                    selectedRect = exitGameRect;
                }
                else if( CompareRect(selectedRect, exitGameRect) )
                {
                    selectedRect = newGameRect;
                }
            }
            else if(event.key.keysym.sym == SDLK_RETURN)
            {
                if( CompareRect(selectedRect, newGameRect) )
                {
                    changedState = GameWindow::GS_PlayState;
                }
                else if( CompareRect(selectedRect, exitGameRect) )
                {
                    *quit = true;
                }
            }
        }

        /// registrovati pomeraj misem preko opcije
        /*if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                if( IfMouseOverRect(event, pvpRect) )
                {
                    *gamestate = GameWindow::GS_PlayStatePvP;
                }
                else if( IfMouseOverRect(event, exitRect) )
                {
                    *quit = true;
                }


            }
        }*/

        if( event.type == SDL_QUIT )
        {
            *quit = true;
        }
    }
}

void ChooseWindow::handle_logic()
{

}

void ChooseWindow::handle_rendering()
{
    SDL_FillRect(screen, &screen->clip_rect, 0x000000);

    ApplySurface( newGameRect.x, newGameRect.y, newGameSurface, screen );
    ApplySurface( exitGameRect.x, exitGameRect.y, exitGameSurface, screen );

    DrawRectangle(selectedRect, screen, 3, borderColorBlue);

    if( SDL_Flip(screen) == -1 )
    {
        throw std::string("flip problem");
    }
}

bool ChooseWindow::ChangedState()
{
    return changedState != GameWindow::GS_ChooseState;
}
