#include "goplayer.h"
#include <QMessageBox>

// HumanGoPlayer

HumanGoPlayer::HumanGoPlayer(stone stoneClr)
{
    /*
        Initiaize human go player
    */
    stoneColor = stoneClr;
}

void HumanGoPlayer::setMove(GoMove mv)
{
    /*
        Set go move for human go player, used by parse input in gogamewidget
    */
    moveStorage = mv;
    giveMove();
}

void HumanGoPlayer::giveMove()
{
    /*
        Give move, used by get input in gogamewidget
    */
    if (moveStorage.isSet)
    {
        if (socket != nullptr)
        {
            if (moveStorage.mt == place)
            {
                QString str = "pl ";
                str += QString::number(moveStorage.pos.position[0]) + " " + QString::number(moveStorage.pos.position[1]);
                socket->write(str.toUtf8());
            }
            else if (moveStorage.mt == pass)
            {
                QString str = "ps";
                socket->write(str.toUtf8());
            }
            else if (moveStorage.mt == resign)
            {
                QString str = "re";
                socket->write(str.toUtf8());
            }
            else
            {
                throw(0);
            }
        }
        moveSeted(moveStorage);
    }
}

void HumanGoPlayer::setSocket(QTcpSocket *socket)
{
    /*
        Set socket if it is multiplayer
    */
    HumanGoPlayer::socket = socket;
}

void HumanGoPlayer::setupRemoveDeadStones()
{
    /*
        Setup for process for removing dead stones
    */
    if (!removeMessage && socket != nullptr)
    {
        QString str = "beginRemoveMessage";
        qDebug() << "Human Go Player Sent Init to TCP";
        socket->write(str.toUtf8());
        removeMessage = true;
        getRemoveDeadStones();
    }
    else if (socket == nullptr)
    {
        getRemoveDeadStones();
        deadStonesSetted();
    }
}

void HumanGoPlayer::recievedRemoveReadySignal()
{
    /*
        Recieved "Ready" signal from tcp go player, start message
    */
    QByteArray datas = socket->readAll();
    QString str = QString(datas);
    if (str == QString("Ready"))
    {
        getRemoveDeadStones();
    }
}

void HumanGoPlayer::getRemoveDeadStones()
{
    /*
        Show message box for player's choice
    */
    QMessageBox acceptStoneRemoval;
    acceptStoneRemoval.setText(stoneColor == black ? "Black: Accept Stone Removal?" : "White: Accept Stone Removal?");
    acceptStoneRemoval.addButton("Continue Play", QMessageBox::ResetRole);
    acceptStoneRemoval.addButton("No", QMessageBox::RejectRole);
    acceptStoneRemoval.addButton("Yes", QMessageBox::AcceptRole);
    int response = acceptStoneRemoval.exec();
    if (response == 2)
    {
        choice = accept;
    }
    else if (response == 1)
    {
        choice = reject;
    }
    else if (response == 0)
    {
        choice = continuePlay;
    }
    else
    {
        qDebug() << response;
        return;
    }
    if (socket != nullptr)
    {
        QString str;
        if (choice == accept)
        {
            str = "accept";
        }
        else if (choice == reject)
        {
            str = "reject";
        }
        else if (choice == continuePlay)
        {
            str = "continue";
        }
        socket->write(str.toUtf8());
    }
}

void HumanGoPlayer::endRemoveDeadStones()
{
    /*
        End remove dead stone, set everything back to initial
    */
    removeMessage = false;
}

// HumanTCPGoPlayer

HumanTcpGoPlayer::HumanTcpGoPlayer(stone stoneClr)
{
    /*
        Initiaize tcp go player
    */
    stoneColor = stoneClr;
}

void HumanTcpGoPlayer::setMove(GoMove mv)
{
    /*
        Set move
    */
    moveStorage = mv;
    giveMove();
}

void HumanTcpGoPlayer::giveMove()
{
    /*
        Give move tcp go player
    */
    if (moveStorage.isSet)
    {
        moveSeted(moveStorage);
    }
    moveStorage = GoMove();
}

void HumanTcpGoPlayer::setSocket(QTcpSocket *socket)
{
    /*
        Set socket tcp go player
    */
    HumanTcpGoPlayer::socket = socket;
    connect(socket, &QTcpSocket::readyRead, this, &HumanTcpGoPlayer::getMoveFromSocket);
}

void HumanTcpGoPlayer::getMoveFromSocket()
{
    /*
        Get move from socket for tcp go player
    */
    QByteArray datas = socket->readAll();
    QString str = QString(datas);
    qDebug() << "Str:";
    qDebug() << str;
    if (str == QString("beginRemoveMessage"))
    {
        if (!removeMessage)
        {
            removeMessage = true;
            qDebug() << "TCP Recieved and Setup";
            setupRemoveDeadStones();
        }
        return;
    }
    QStringList list = str.split(" ", QString::SkipEmptyParts);
    if (!list.length())
    {
        return;
    }
    if (list[0] == "pl")
    {
        int i = list[1].toInt(), j = list[2].toInt();
        setMove(GoMove(place, Pos(i, j)));
    }
    else if (list[0] == "ps")
    {
        setMove(GoMove(pass, Pos(0, 0)));
    }
    else if (list[0] == "re")
    {
        setMove(GoMove(resign, Pos(0, 0)));
    }
}

void HumanTcpGoPlayer::setupRemoveDeadStones()
{
    /*
        Setup remove dead stones for tcp go player
    */
    qDebug() << "Setup remove dead stone tcp";
    connect(socket, &QTcpSocket::readyRead, this, &HumanTcpGoPlayer::getRemoveDeadStonesSocket);
    disconnect(socket, &QTcpSocket::readyRead, this, &HumanTcpGoPlayer::getMoveFromSocket);
    hideEndButton();
}

void HumanTcpGoPlayer::getRemoveDeadStones()
{
    /*
        Empty because tcp go player uses signal slot connection
    */
}

void HumanTcpGoPlayer::getRemoveDeadStonesSocket()
{
    /*
        Get human player's choice from socket
    */
    QByteArray datas = socket->readAll();
    QString str = QString(datas);
    if (str == "accept")
    {
        choice = accept;
        beginDeadStoneMessage();
        deadStonesSetted();
    }
    else if (str == "reject")
    {
        choice = reject;
        beginDeadStoneMessage();
        deadStonesSetted();
    }
    else if (str == "continue")
    {
        choice = continuePlay;
        beginDeadStoneMessage();
        deadStonesSetted();
    }
    else
    {
        qDebug() << "getRemoveDeadStonesSocket from string unknown";
        qDebug() << str;
    }
}

void HumanTcpGoPlayer::endRemoveDeadStones()
{
    /*
        End remove dead stone, set everything back to initial
    */
    connect(socket, &QTcpSocket::readyRead, this, &HumanTcpGoPlayer::getMoveFromSocket);
    disconnect(socket, &QTcpSocket::readyRead, this, &HumanTcpGoPlayer::getRemoveDeadStonesSocket);
    removeMessage = false;
}

// ComputerGoPlayerEasy !NOTFINISHED!

ComputerGoPlayerEasy::ComputerGoPlayerEasy(stone stoneClr)
{
    ComputerGoPlayerEasy::stoneColor = stoneClr;
}

void ComputerGoPlayerEasy::giveMove()
{
    return;
}

void ComputerGoPlayerEasy::setMove(GoMove mv)
{
}

void ComputerGoPlayerEasy::setSocket(QTcpSocket *socket)
{
}

void ComputerGoPlayerEasy::setupRemoveDeadStones()
{
}

void ComputerGoPlayerEasy::getRemoveDeadStones()
{
}

void ComputerGoPlayerEasy::endRemoveDeadStones()
{
}
