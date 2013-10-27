#ifndef PLAYWINDOW_HPP
#define PLAYWINDOW_HPP

#include <iostream>
#include <fstream>

#include "GameWindow.hpp"
#include "StatusBar.hpp"
#include "GameField.hpp"

typedef struct
{
    int levelNum;
    int cardNum;
} LevelConf;

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
    int currentLevel;

    int statusBarWidth;
    int movesNum;

    bool gameDone;

    StatusBar *_statusBar;
    GameField *_gameField;

    void InitializeLevel(int);

    std::vector<LevelConf> levelconfs;
};

#endif


