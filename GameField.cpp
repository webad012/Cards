#include "GameField.hpp"

GameField::GameField(SDL_Surface *screen, int statusBarWidth, int cardNum, int movesNum)
: numOfCards(cardNum), numberOfMoves(movesNum), levelDone(false)
{
    fieldRect.x = 0;
    fieldRect.y = 0;
    fieldRect.w = screen->w - statusBarWidth;
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
    gameDoneSurface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_ANYFORMAT, fieldRect.w, fieldRect.h, screen->format->BitsPerPixel, 0, 0, 0, 0);
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

                std::vector<Card*> tempcards;
                for(int i=0; i<numOfCards; i++)
                {
                   if( i == chosen_positions[0] )
                   {
                       tempcards.push_back(cards[ chosen_positions[1] ]);
                   }
                   else if( i == chosen_positions[1] )
                   {
                       tempcards.push_back(cards[ chosen_positions[0] ]);
                   }
                   else
                   {
                       tempcards.push_back(cards[ i ]);
                   }
                }

                for(int i=0; i<numOfCards; i++)
                {
                   cards[i] = tempcards[i];
                }

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

    /// logic for level done
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
            levelDone = true;
        }
    }

    /// logic for game done
    {
        int numOfCardsPerRow = ceil( sqrt(numOfCards) );

        for(int i=0; i<numOfCards; i++)
        {
           if( cards[i]->IsRemoved() == false )
           {
               for(int j=i+1; j<numOfCards; j++)
               {
                   if( cards[i]->GetNum() == cards[j]->GetNum() )
                   {
                       if( !PathExist(cards[i]->GetCoordJ()*numOfCardsPerRow+cards[i]->GetCoordI(), cards[j]->GetCoordJ()*numOfCardsPerRow+cards[j]->GetCoordI()) )
                       {
                           *gameDone = true;
                       }
                   }
               }
           }
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

bool GameField::LevelPassed()
{
    return levelDone;
}

bool GameField::PathExist(int c1, int c2)
{
    int numOfCardsPerRow = ceil( sqrt(numOfCards) );
    int numOfCardsPerColumn = floor( sqrt(numOfCards) );

    // Mark all the vertices as not visited
    bool *visited = new bool[numOfCards];
    for(int i=0; i<numOfCards; i++)
    {
        visited[i] = false;
    }

    // Create a queue for BFS
    std::list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[c1] = true;
    queue.push_back(c1);


    while (!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        int s = queue.front();
        queue.pop_front();


        int topi, topj, bottomi, bottomj, lefti, leftj, righti, rightj;

        topi = cards[s]->GetCoordI();
        topj = cards[s]->GetCoordJ()-1;

        bottomi = cards[s]->GetCoordI();
        bottomj = cards[s]->GetCoordJ()+1;

        lefti = cards[s]->GetCoordI()-1;
        leftj = cards[s]->GetCoordJ();

        righti = cards[s]->GetCoordI()+1;
        rightj = cards[s]->GetCoordJ();

        if( topj >= 0 )
        {
            int loc = topj*numOfCardsPerRow + topi;

            if(loc == c2)
            {
                return true;
            }
            else if (!visited[loc] && (!cards[loc]->IsRemoved() || !cards[loc]->IsHole()) )
            {
                visited[loc] = true;
                queue.push_back(loc);
            }
        }

        if( bottomj <= numOfCardsPerColumn-1 )
        {
            int loc = bottomj*numOfCardsPerRow + bottomi;

            if(loc == c2)
            {
                return true;
            }
            else if(!visited[loc] && (!cards[loc]->IsRemoved() || !cards[loc]->IsHole()) )
            {
                visited[loc] = true;
                queue.push_back(loc);
            }
        }

        if( lefti >= 0 )
        {
            int loc = leftj*numOfCardsPerRow + lefti;

            if(loc == c2)
            {
                return true;
            }
            else if(!visited[loc] && (!cards[loc]->IsRemoved() || !cards[loc]->IsHole()) )
            {
                visited[loc] = true;
                queue.push_back(loc);
            }
        }

        if( righti <= numOfCardsPerRow-1 )
        {
            int loc = rightj*numOfCardsPerRow + righti;

            if(loc == c2)
            {
                return true;
            }
            else if(!visited[loc] && (!cards[loc]->IsRemoved() || !cards[loc]->IsHole()) )
            {
                visited[loc] = true;
                queue.push_back(loc);
            }
        }
    }

    for(int j=0; j<numOfCardsPerColumn; j++)
    {
        for(int i=0; i < numOfCardsPerRow; i++)
        {
            if(cards[j*numOfCardsPerRow + i]->IsRemoved())
            {
                std::cout << "NULL";
            }
            else
            {
                std::cout << cards[j*numOfCardsPerRow + i]->GetNum();
            }

            std::cout << " ";
        }
        std::cout << std::endl;
    }
    return false;
}





