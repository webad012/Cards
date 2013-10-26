#include "GameField.hpp"

GameField::GameField(SDL_Surface *screen, int statusBarHeight)
: numberOfMoves(0)
{
    fieldRect.x = 0;
    fieldRect.y = statusBarHeight;
    fieldRect.w = screen->w;
    fieldRect.h = screen->h;

    fieldBorderRect.x = fieldRect.x + 20;
    fieldBorderRect.y = fieldRect.y + 20;
    fieldBorderRect.w = fieldRect.w - fieldBorderRect.x - 20;
    fieldBorderRect.h = fieldRect.h - fieldBorderRect.y - 20;

    ColorBlack.r = 0;
    ColorBlack.g = 0;
    ColorBlack.b = 0;
    ColorBlack.unused = 255;

    ColorWhite.r = 255;
    ColorWhite.g = 255;
    ColorWhite.b = 255;
    ColorWhite.unused = 255;

    fieldBorderSize = 2;

    numOfCards = 12;
    int numOfHoles = ceil(numOfCards/3);

    numOfFacedUp = 0;

    int numOfCardsPerRow = ceil( sqrt(numOfCards) );
    int numOfCardsPerColumn = floor( sqrt(numOfCards) );
    int cardHeight = (fieldBorderRect.h / (numOfCardsPerColumn)) * 4/5;
    int heightSpace = (fieldBorderRect.h - cardHeight*numOfCardsPerColumn)/(numOfCardsPerColumn+1);
    int cardWidth =  cardHeight/2;
    int widthSpace = (fieldBorderRect.w - cardWidth*numOfCardsPerRow)/(numOfCardsPerRow+1);

    std::vector<int> rand_nums;
    for(int i=0; i<numOfCards; i++)
    {
        rand_nums.push_back(i%(numOfCards/2));
    }
    std::srand ( unsigned ( time(0) ) );
    std::random_shuffle(rand_nums.begin(), rand_nums.end());

    for(int j=0; j<numOfCardsPerColumn; j++)
    {
        SDL_Rect cardRect;
        cardRect.y = fieldBorderRect.y + heightSpace + (j * (cardHeight+heightSpace));
        cardRect.w = cardWidth;
        cardRect.h = cardHeight;

        for(int i=0; i < numOfCardsPerRow; i++)
        {
            cardRect.x = fieldBorderRect.x + widthSpace + (i * (cardWidth+widthSpace));

            cards.push_back( new Card(rand_nums.back(), cardRect, i, j) );
            rand_nums.pop_back();
        }
    }

    int num_of_holes_left = numOfHoles;
    while(num_of_holes_left>0)
    {
        int rnd = rand() % numOfCards;

        if( cards[rnd]->IsHole() == false )
        {
            cards[rnd]->SetHole();
            num_of_holes_left--;
        }
    }

    flip_time = 1000;

    /// game done part initialization
    gameDoneSurface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_ANYFORMAT, screen->w, screen->h, screen->format->BitsPerPixel, 0, 0, 0, 0);
    if( gameDoneSurface == NULL )
    {
        throw std::string(" - GameField::GameField - gameDoneSurface problem");
    }
    SDL_FillRect(gameDoneSurface, NULL, SDL_MapRGBA(screen->format, 0xAA, 0xAA, 0xAA, 0xFF));
	SDL_SetAlpha(gameDoneSurface, SDL_SRCALPHA, 100);
    gameDoneLastTime = SDL_GetTicks();
    gameDonePressEnterDraw = true;
    gameDoneBlinkTime = 800;
    restartFont = TTF_OpenFont( "CardsFont.ttf", 50 );
    if( restartFont == NULL )
    {
        throw std::string(" - GameField::GameField - restartFont problem - " + std::string(TTF_GetError()));
    }
    restartFontSurface = TTF_RenderUTF8_Shaded(restartFont, "Pritisni Enter", ColorWhite, ColorBlack);
    if( restartFontSurface == NULL )
    {
        throw std::string(" - GameField::GameField - restartFontSurface problem");
    }
    restartFontRect.x = (screen->w - restartFontSurface->w)/2;
    restartFontRect.y = (screen->h - restartFontSurface->h)/2;
}

GameField::~GameField()
{
    for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
    {
        delete (*it);
    }

    SDL_FreeSurface(gameDoneSurface);
    SDL_FreeSurface(restartFontSurface);

    TTF_Quit();
    SDL_Quit();
}

void GameField::handle_events(SDL_Event event)
{
    if( (event.type == SDL_MOUSEBUTTONDOWN) )
    {
        if( (event.button.button == SDL_BUTTON_LEFT) && (numOfFacedUp < 2) )
        {
            for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
            {
                SDL_Rect cardRect = (*it)->GetRect();
                if( ( event.button.x > cardRect.x ) && ( event.button.x< cardRect.x + cardRect.w )
                   && ( event.button.y > cardRect.y ) && ( event.button.y < cardRect.y + cardRect.h ) )
                {
                    (*it)->HandleLeftMouseClick(event);
                }
            }
        }

        if( (event.button.button == SDL_BUTTON_RIGHT) )
        {
            for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
            {
                SDL_Rect cardRect = (*it)->GetRect();
                if( ( event.button.x > cardRect.x ) && ( event.button.x< cardRect.x + cardRect.w )
                   && ( event.button.y > cardRect.y ) && ( event.button.y < cardRect.y + cardRect.h ) )
                {
                    (*it)->HandleRightMouseClick(event);
                }
            }
        }
    }
}

void GameField::handle_logic(bool *gameDone, int *movesNum)
{
    /// logic for face up
    {
        int num_of_faced_up_cards = 0;
        int faced_up_positions[2];
        int i=0;
        for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
        {
           if( (*it)->GetFacedUp() == true )
           {
               faced_up_positions[num_of_faced_up_cards] = i;
               num_of_faced_up_cards++;
           }
           i++;
        }

        if(num_of_faced_up_cards == 2)
        {
            if( cards[ faced_up_positions[0] ]->GetNum() == cards[ faced_up_positions[1] ]->GetNum()
               && (cards[ faced_up_positions[0] ]->CloseForAction( cards[ faced_up_positions[1] ]) == true) )
            {
                if(numOfFacedUp == 1)
                {
                    start_time = SDL_GetTicks();
                }
                Uint32 this_time = SDL_GetTicks();
                if(this_time - start_time >= flip_time)
                {
                    cards[ faced_up_positions[0] ]->Remove();
                    cards[ faced_up_positions[1] ]->Remove();
                    numberOfMoves++;
                }
            }
            else
            {
                if(numOfFacedUp == 1)
                {
                    start_time = SDL_GetTicks();
                }

                /// blinking text
                Uint32 this_time = SDL_GetTicks();
                if(this_time - start_time >= flip_time)
                {
                    for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
                    {
                       (*it)->SetFacedDown();
                    }
                    numberOfMoves++;
                }
            }
        }
        numOfFacedUp = num_of_faced_up_cards;
    }

    /// logic for chosen cards
    {
        int num_of_chosen_cards = 0;
        int chosen_positions[2];
        int i=0;
        for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
        {
           if( (*it)->GetChosen() == true )
           {
               chosen_positions[num_of_chosen_cards] = i;
               num_of_chosen_cards++;
           }
           i++;
        }

        if(num_of_chosen_cards == 2)
        {
            if( cards[ chosen_positions[0] ]->CloseForAction( cards[ chosen_positions[1] ] ) == true )
            {
                cards[ chosen_positions[0] ]->Swap( cards[ chosen_positions[1] ] );
                numberOfMoves++;
            }
            else
            {
                for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
                {
                   (*it)->SetUnchosen();
                }
            }
        }
    }

    /// logic for game done
    {
        int num_of_removed = 0;
        for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
        {
           if( (*it)->IsRemoved() == true )
           {
               num_of_removed++;
           }
        }

        if(numOfCards == num_of_removed)
        {
            *gameDone = true;
        }
    }

    *movesNum = numberOfMoves;
}

void GameField::handle_rendering(bool gameDone, SDL_Surface *screen)
{
    SDL_FillRect(screen, &fieldRect, SDL_MapRGB(screen->format, 0xAA, 0xAA, 0xAA));

    // draw field border
    for(int i=0; i < fieldBorderSize; i++)
    {
        rectangleRGBA(screen, fieldBorderRect.x + i, fieldBorderRect.y + i, fieldBorderRect.x + fieldBorderRect.w - i, fieldBorderRect.y + fieldBorderRect.h - i,
                      ColorBlack.r, ColorBlack.g, ColorBlack.b, ColorBlack.unused);
    }

    for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
    {
        (*it)->handle_rendering(screen);
    }

    if(gameDone)
    {
        SDL_BlitSurface(gameDoneSurface, NULL, screen, &fieldRect);

        /// blinking text
        Uint32 this_time = SDL_GetTicks();
        if(this_time - gameDoneLastTime >= gameDoneBlinkTime)
        {
            gameDoneLastTime = this_time;
            gameDonePressEnterDraw = !gameDonePressEnterDraw;
        }
        if(gameDonePressEnterDraw == true)
        {
            SDL_BlitSurface(restartFontSurface, NULL, screen, &restartFontRect);
        }
    }

    if( SDL_Flip(screen) == -1 )
    {
        throw std::string("flip problem");
    }
}


