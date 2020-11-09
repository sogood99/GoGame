#ifndef GOBOARD_H
#define GOBOARD_H

#include "utils.h"

enum capturedState: int{
    /*
        Caputured state used in caputured tuple
    */
    notAccessed = 0,
    notCaptured = 1,
    Captured = 2
};

class CapturedTuple{
    /*
        Class containing everything needed in detecting captured stones
    */
public:
    bool isCaptured = false;
    int captureNum = 0;
    QVector<QVector<capturedState>> capturedVector;
public:
    CapturedTuple(size boardSize);
    CapturedTuple(size boardSize, bool tfCaptured);
    ~CapturedTuple(){}
    void setBool(bool tf);
    void setCaptured(Pos p);
    void setNotCaptured(Pos p);
    capturedState getState(Pos p);
    void append(CapturedTuple other);//add other's captured to self
    int countCaptured();
};

class boolInt{
public:
    bool boolean;
    int integer;
public:
    boolInt(bool inptbl, int inptn){
        boolean = inptbl;
        integer = inptn;
    }
    boolInt(bool inptbl){
        boolean = inptbl;
        integer = 0;
    }
};

class GoBoard{
    /*
        Class containing everything needed for a functioning go board
    */
public:
    size boardSize;
    QVector<QVector<stone>> currentBoard;
    QVector<QVector<stone>> lastBoard;
public:
    GoBoard(size boardSize = nineteen);
    ~GoBoard(){}
    void putStone(Pos p, stone stn);// putStone puts stone without caring if it is valid
    boolInt placeStone(Pos p, stone stn);// placeStone puts stone and caring if it is valid, uses put stone
    bool canPlaceStone(Pos p, stone stn);
    QVector<QVector<stone>> floodFill(QVector<QVector<DeadStone>> deadStones);
    void displayBoardConsole();
    void displayLastBoardConsole();
protected:
    QVector<Pos> neighbor(Pos p);
    bool positionInBoard(Pos p);
    bool positionIsEmpty(Pos p);
    bool isKo(Pos p, stone stn);
    bool isSuicide(Pos p, stone stn);
    bool isEye(Pos p, stone stn);
    void setLastBoard();
    CapturedTuple captureOther(Pos p, stone stn); // place stone as position to capture other
    CapturedTuple capture(Pos p, stone stn);// for a stone in a group that can capture, used by capture other (DFS)
    void removeCapturedStones(CapturedTuple capturedTup);
private:
    void DFSFloodFill(int i,int j, QVector<QVector<bool>> &accessed,QVector<QVector<stone>> &filled, QVector<QVector<DeadStone>> deadStones);
};

#endif // GOBOARD_H
