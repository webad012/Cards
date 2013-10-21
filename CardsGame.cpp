#include "CardsGame.hpp"

CardsGame::CardsGame()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        throw std::string("init problem");
    }

    if( TTF_Init() == -1 )
    {
         throw std::string("font init problem");
    }

    screen = SDL_SetVideoMode(600, 630, 32, SDL_SWSURFACE );
    if( screen == NULL )
    {
        throw std::string("screen init problem");;
    }

    SDL_WM_SetCaption( "Karte", NULL );

    fieldBorderRect.x = 20;
    fieldBorderRect.y = 20;
    fieldBorderRect.w = screen->w - fieldBorderRect.x - 20;
    fieldBorderRect.h = screen->h - fieldBorderRect.y - 20;

    fieldBorderColor.r = 0;
    fieldBorderColor.g = 0;
    fieldBorderColor.b = 0;
    fieldBorderColor.unused = 255;

    fieldBorderSize = 2;

    numOfCards = 25;
    numOfFacedUp = 0;

    numOfCardsPerRow = ceil( sqrt(numOfCards) );
    numOfCardsPerColumn = floor( sqrt(numOfCards) );
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

    flip_time = 1000;
}

CardsGame::~CardsGame()
{
    for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
    {
        delete (*it);
    }

    TTF_Quit();
    SDL_Quit();
}

void CardsGame::handle_events(bool *quit)
{
    while( SDL_PollEvent( &event ) )
    {
        if( (event.type == SDL_MOUSEBUTTONDOWN) )
        {
            if( (event.button.button == SDL_BUTTON_LEFT) && (numOfFacedUp < 2) )
            {
                for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
                {
                    (*it)->HandleLeftMouseClick(event);
                }
            }

            if( (event.button.button == SDL_BUTTON_RIGHT) )
            {
                for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
                {
                    (*it)->HandleRightMouseClick(event);
                }
            }
        }

        if( event.type == SDL_QUIT )
        {
            *quit = true;
        }
    }
}

void CardsGame::handle_logic()
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
                    //delete cards[ faced_up_positions[1] ];
                    //cards.erase( cards.begin()+faced_up_positions[1] );
                    //delete cards[ faced_up_positions[0] ];
                    //cards.erase( cards.begin()+faced_up_positions[0] );
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
}

void CardsGame::handle_rendering()
{
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xAA, 0xAA, 0xAA));

    // draw field border
    DrawRectangle(fieldBorderRect, screen, fieldBorderSize, fieldBorderColor);

    for(std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); it++)
    {
        (*it)->handle_rendering(screen);
    }

    if( SDL_Flip(screen) == -1 )
    {
        throw std::string("flip problem");
    }
}

void CardsGame::DrawRectangle(SDL_Rect rect, SDL_Surface *destination, int bordersize, SDL_Color bordercolor)
{
    for(int i=0; i < bordersize; i++)
    {
        rectangleRGBA(destination, rect.x + i, rect.y + i, rect.x + rect.w - i, rect.y + rect.h - i, bordercolor.r, bordercolor.g, bordercolor.b, bordercolor.unused);
    }
}


