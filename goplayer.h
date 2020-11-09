#ifndef GOPLAYER_H
#define GOPLAYER_H

#include "goboard.h"
#include <QObject>
#include <QTcpSocket>

class GoPlayer: public QObject{
    /*
        Template class for different players
    */
    Q_OBJECT
public:
    stone stoneColor;
    RemoveDeadStoneChoice choice = notSet;
public:
    ~GoPlayer(){};
    virtual void giveMove() = 0;
    virtual void setMove(GoMove mv) = 0;
    virtual void setSocket(QTcpSocket* socket) = 0;
    virtual void setupRemoveDeadStones() = 0;
    virtual void endRemoveDeadStones() = 0;
signals:
    void moveSeted(GoMove mv);
    void deadStonesSetted();
    void beginDeadStoneMessage();
    void hideEndButton();
public slots:
    virtual void getRemoveDeadStones() = 0;
protected:
    GoMove moveStorage;
    QTcpSocket* socket = nullptr;
    bool removeMessage = false;
};

class HumanGoPlayer: public GoPlayer{
    /*
        Class for human go players
    */
    Q_OBJECT
public:
    HumanGoPlayer(stone stoneColor);
    ~HumanGoPlayer(){}
    void giveMove() override;
    void setMove(GoMove mv) override;
    void setSocket(QTcpSocket *socket) override;
    void setupRemoveDeadStones() override;
    void endRemoveDeadStones() override;
public slots:
    void getRemoveDeadStones() override;
    void recievedRemoveReadySignal();
protected:
};

class HumanTcpGoPlayer: public GoPlayer{
    /*
        Class for other go player in multiplayer
    */
    Q_OBJECT
public:
    HumanTcpGoPlayer(stone stoneColor);
    void giveMove() override;
    void setMove(GoMove mv) override;
    void setSocket(QTcpSocket *socket) override;
    void setupRemoveDeadStones() override;
    void endRemoveDeadStones() override;
public slots:
    void getMoveFromSocket();
    void getRemoveDeadStones() override;
    void getRemoveDeadStonesSocket();
protected:

};

class ComputerGoPlayerEasy: public GoPlayer{
    /*
        Unfinished computer go player class
    */
    Q_OBJECT
public:
    ComputerGoPlayerEasy(stone stoneColor);
    ~ComputerGoPlayerEasy(){}
    void giveMove() override;
    void setMove(GoMove mv) override;
    void setSocket(QTcpSocket *socket) override;
    void setupRemoveDeadStones() override;
    void endRemoveDeadStones() override;
public slots:
    void getRemoveDeadStones() override;
};

#endif // GOPLAYER_H
