#ifndef UTILS_H
#define UTILS_H

#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <QVector>
#include <QString>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QDebug>
#include <QSound>
#include <QTextEdit>
#include <QRadioButton>
#include <QLineEdit>
#include <QStatusBar>

/*
    Header for most include and useful classes/functions
*/

enum stone: int{
    black = 1,
    white = -1,
    noStone = 0
};

enum DeadStone{
    isDead = 1,
    notDead = 0
};

enum RemoveDeadStoneChoice{
    /*
        Enum for last step of removing dead stones
    */
    notSet,
    continuePlay,
    reject,
    accept
};

stone otherStone(stone stn);

enum size: int{
    /*
        Enum for board size
    */
    nine = 9,
    thirteen = 13,
    nineteen = 19
};

class Pos{
    /*
        Class for position on board
    */
public:
    QVector<int> position = QVector<int>(2, 0);
public:
    Pos(int x, int y);
    ~Pos(){}
    void setPosition(int x, int y);
};

enum moveType{
    /*
        Enum for different moves
    */
    resign = -1,
    pass = 0,
    place = 1
};

class GoMove{
    /*
        Class for moves
    */
public:
    bool isSet = false;
    moveType mt;
    Pos pos = Pos(-1, -1);
public:
    GoMove();
    GoMove(moveType mvtp, Pos pos);
};

#endif
