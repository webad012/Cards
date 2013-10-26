#include "Card.hpp"

Card::Card(int num, SDL_Rect rect, int i, int j)
: cardRect(rect), cardNum(num), facedUp(false), chosen(false), removed(false), coordi(i), coordj(j)
{
    colorBlack.r = 0;
    colorBlack.g = 0;
    colorBlack.b = 0;

    cardNumFont = TTF_OpenFont( "CardsFont.ttf", cardRect.h*3/5 );
    if( cardNumFont == NULL )
    {
        throw std::string("Card::Card - cardNumFont problem - " + std::string(TTF_GetError()));
    }

    std::stringstream ss;
    ss << cardNum;
    cardNumSurface = TTF_RenderUTF8_Blended(cardNumFont, ss.str().c_str(), colorBlack);
}

Card::~Card()
{
    SDL_FreeSurface(cardNumSurface);
    TTF_CloseFont(cardNumFont);
}

void Card::HandleLeftMouseClick(SDL_Event e)
{
    facedUp = true;
}

void Card::HandleRightMouseClick(SDL_Event e)
{
    chosen = true;
}

void Card::handle_rendering(SDL_Surface *screen)
{
    if(removed == true)
    {
        if(hole == true)
        {
            SDL_FillRect(screen, &cardRect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
        }
        else
        {
            rectangleRGBA(screen, cardRect.x, cardRect.y, cardRect.x + cardRect.w, cardRect.y + cardRect.h, 0x00, 0x00, 0x00, 0xFF);
        }
    }
    else
    {
        if(facedUp == false)
        {
            SDL_FillRect(screen, &cardRect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x6D));
            rectangleRGBA(screen, cardRect.x + 3, cardRect.y + 3, cardRect.x + cardRect.w - 3, cardRect.y + cardRect.h - 3, 0xFF, 0xFF, 0xFF, 0xFF);
        }
        else if(facedUp == true)
        {
            SDL_FillRect(screen, &cardRect, SDL_MapRGB(screen->format, 0xCC, 0xCC, 0xCC));
            rectangleRGBA(screen, cardRect.x + 3, cardRect.y + 3, cardRect.x + cardRect.w - 3, cardRect.y + cardRect.h - 3, 0x00, 0x00, 0x00, 0xFF);
            SDL_Rect cardNumOffset;
            cardNumOffset.x = cardRect.x; // + (cardNumSurface->w)/2;
            cardNumOffset.y = cardRect.y; // + (cardNumSurface->h)/2;
            SDL_BlitSurface(cardNumSurface, NULL, screen, &cardNumOffset);
        }

        if(chosen == true)
        {
            rectangleRGBA(screen, cardRect.x -1, cardRect.y -1, cardRect.x + cardRect.w +1, cardRect.y + cardRect.h +1, 0x00, 0x00, 0xFF, 0xFF);
            rectangleRGBA(screen, cardRect.x -2, cardRect.y -2, cardRect.x + cardRect.w +2, cardRect.y + cardRect.h +2, 0x00, 0x00, 0xFF, 0xFF);
            rectangleRGBA(screen, cardRect.x -3, cardRect.y -3, cardRect.x + cardRect.w +3, cardRect.y + cardRect.h +3, 0x00, 0x00, 0xFF, 0xFF);
        }
    }
}

int Card::GetNum()
{
    return cardNum;
}

bool Card::GetFacedUp()
{
    return facedUp;
}

void Card::SetFacedDown()
{
    if(facedUp == true)
    {
        facedUp = false;
    }
}

int Card::GetCoordI()
{
    return coordi;
}

int Card::GetCoordJ()
{
    return coordj;
}

bool Card::CloseForAction(Card *c)
{
    if( ((coordi == c->GetCoordI()) && (abs( coordj - c->GetCoordJ() ) == 1))
       || ((coordj== c->GetCoordJ()) && (abs( coordi - c->GetCoordI() ) == 1)) )
    {
        if( (removed == true && hole == true)
           || (c->IsRemoved() == true && c->IsHole() == true) )
        {
           return false;
        }

        return true;
    }

    return false;
}

bool Card::IsRemoved()
{
    return removed;
}

bool Card::GetChosen()
{
    return chosen;
}

void Card::Swap(Card *c)
{
    SDL_Rect tempRect;
    int tempi, tempj;

    tempRect = c->GetRect();
    tempi = c->GetCoordI();
    tempj = c->GetCoordJ();

    c->SetRect(cardRect);
    c->SetCoordI(coordi);
    c->SetCoordJ(coordj);

    cardRect = tempRect;
    coordi = tempi;
    coordj = tempj;

    chosen = false;
    c->SetUnchosen();
}

void Card::SetUnchosen()
{
    chosen = false;
}

SDL_Rect Card::GetRect()
{
    return cardRect;
}

void Card::SetRect(SDL_Rect r)
{
    cardRect = r;
}

void Card::SetCoordI(int i)
{
    coordi = i;
}

void Card::SetCoordJ(int j)
{
    coordj = j;
}

void Card::Remove()
{
    removed = true;
    facedUp = false;
    chosen = false;
}

bool Card::IsHole()
{
    return hole;
}

void Card::SetHole()
{
    hole = true;
}




