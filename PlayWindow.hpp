#ifndef PLAYWINDOW_HPP
#define PLAYWINDOW_HPP

#include <iostream>

#include "GameWindow.hpp"
#include "StatusBar.hpp"
#include "GameField.hpp"

class PlayWindow : public GameWindow
{
public:
    PlayWindow();
    ~PlayWindow();

    void handle_events(bool*);
    void handle_logic();
    void handle_rendering();

    bool ChangedState();

private:
    int statusBarHeight;
    int movesNum;

    bool gameDone;

    StatusBar *_statusBar;
    GameField *_gameField;
};

#endif


