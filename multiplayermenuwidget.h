#ifndef MULTIPLAYERMENUWIDGET_H
#define MULTIPLAYERMENUWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include "utils.h"
#include <QPushButton>
#include <QNetworkInterface>
#include "gogamewidget.h"

class MultiplayerMenuWidget: public QWidget
{
    Q_OBJECT
    /*
        Multiplayer menu class, used to help TCP connections
    */
public:
    MultiplayerMenuWidget(QWidget *parent, QStackedWidget* mainstack);
    ~MultiplayerMenuWidget();

public:
    QWidget* parent = nullptr;
    QStackedWidget* mainstack = nullptr;
    GoGameWidget* game = nullptr;

    QWidget* backgroundWidget = nullptr;
    QTcpServer* server = nullptr;
    QTcpSocket* socket = nullptr;
    QWidget* buttonWidget = nullptr;
    QPushButton* createGameButton = nullptr;
    QPushButton* findGameButton = nullptr;
    QWidget* createGameWidget = nullptr;
    QVBoxLayout* createGameLayout = nullptr;
    QWidget* findGameWidget = nullptr;
    QLabel* ipRead = nullptr;
    QLabel* portRead = nullptr;
    QLineEdit* ipText = nullptr;
    QLineEdit* portText = nullptr;
    QPushButton* exitButton = nullptr;

    QRadioButton* sz_nineteen = nullptr;
    QRadioButton* sz_thirteen = nullptr;
    QRadioButton* sz_nine = nullptr;


public slots:
    void exitButtonPushed();
    void createGamePushed();
    void findGamePushed();
    void hideCreateGame();
    void hideFindGame();
    void findGameButtonPushed();
    void serverConnected();
    void socketRead();

private:
    bool initialSent = false;
};

#endif // MULTIPLAYERMENUWIDGET_H
