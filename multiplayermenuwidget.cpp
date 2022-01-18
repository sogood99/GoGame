#include "multiplayermenuwidget.h"

MultiplayerMenuWidget::MultiplayerMenuWidget(QWidget *parent, QStackedWidget *mainstack)
{
    /*
        Initialize MultiplayerMenuWidget, creategame/findgame
    */
    MultiplayerMenuWidget::parent = parent;
    MultiplayerMenuWidget::mainstack = mainstack;

    backgroundWidget = new QWidget(this);

    buttonWidget = new QWidget(backgroundWidget);
    createGameButton = new QPushButton(buttonWidget);
    findGameButton = new QPushButton(buttonWidget);
    exitButton = new QPushButton(backgroundWidget);

    backgroundWidget->setGeometry(0, 0, 800, 600);
    backgroundWidget->setStyleSheet("background-image:/img/homepageBackground.jpg;"
                                    "background-repeat: no-repeat;");
    backgroundWidget->show();
    createGameButton->setText("Create Game");
    findGameButton->setText("Find Game");
    exitButton->setText("Exit");

    buttonWidget->setStyleSheet("QPushButton{border: 1px solid black;"
                                "border-radius: 20px;"
                                "background-position: bottom center;"
                                "background-color: rgba(255, 255, 255, 250);"
                                "font-size: 20px;"
                                "color: rgba(0, 0, 0, 200);}"
                                "QPushButton:hover{	border: 1px solid black;"
                                "border-radius: 20px;"
                                "background-position: bottom center;"
                                "background-color: rgba(125, 125, 125, 250);"
                                "font-size: 20px;"
                                "color: rgba(250, 250, 250, 250);}");

    buttonWidget->setGeometry(100, 100, 300, 500);
    createGameButton->setGeometry(0, 0, 150, 50);
    findGameButton->setGeometry(0, 100, 150, 50);

    exitButton->setGeometry(100, 300, 100, 50);
    exitButton->setStyleSheet("QPushButton{border: 1px solid black;"
                              "border-radius: 20px;"
                              "background-position: bottom center;"
                              "background-color: rgba(255, 255, 255, 250);"
                              "font-size: 17px;"
                              "color: rgba(0, 0, 0, 200);}"
                              "QPushButton:hover{border: 1px solid black;"
                              "border-radius: 20px;"
                              "background-position: bottom center;"
                              "background-color: rgba(125, 125, 125, 250);"
                              "font-size: 17px;"
                              "color: rgba(250, 250, 250, 250);}");
    exitButton->show();

    findGameWidget = new QWidget(backgroundWidget);
    findGameWidget->setStyleSheet("QPushButton{border: 1px solid black;"
                                  "border-radius: 20px;"
                                  "background-position: bottom center;"
                                  "background-color: rgba(255, 255, 255, 250);"
                                  "font-size: 17px;"
                                  "color: rgba(0, 0, 0, 200);}"
                                  "QPushButton:hover{border: 1px solid black;"
                                  "border-radius: 20px;"
                                  "background-position: bottom center;"
                                  "background-color: rgba(125, 125, 125, 250);"
                                  "font-size: 17px;"
                                  "color: rgba(250, 250, 250, 250);}"
                                  "QLineEdit{"
                                  "background-color: rgba(255, 255, 255, 250);"
                                  "font-size: 17px;"
                                  "color: rgba(0, 0, 0, 200);"
                                  "}");
    findGameWidget->setGeometry(300, 100, 300, 400);
    findGameWidget->hide();

    createGameWidget = new QWidget(backgroundWidget);
    createGameWidget->setGeometry(300, 100, 300, 400);
    createGameWidget->hide();

    ipText = new QLineEdit(findGameWidget);
    ipText->setPlaceholderText("IP Address");
    portText = new QLineEdit(findGameWidget);
    portText->setPlaceholderText("Port");

    QPushButton *findFindButton = new QPushButton("Find", findGameWidget);
    QPushButton *findBackButton = new QPushButton("Back", findGameWidget);

    ipText->setGeometry(0, 0, 250, 50);
    portText->setGeometry(0, 100, 250, 50);
    findFindButton->setGeometry(25, 200, 200, 25);
    findBackButton->setGeometry(25, 250, 200, 25);

    QPushButton *createBackButton = new QPushButton("Back", createGameWidget);
    ipRead = new QLabel(createGameWidget);
    portRead = new QLabel(createGameWidget);

    ipRead->setText("IP Address: ");
    portRead->setText("Port: ");

    sz_nine = new QRadioButton(createGameWidget);
    sz_nine->setText("Nine");
    sz_thirteen = new QRadioButton(createGameWidget);
    sz_thirteen->setText("Thirteen");
    sz_nineteen = new QRadioButton(createGameWidget);
    sz_nineteen->setText("Nineteen");
    sz_nineteen->setChecked(true);

    createGameWidget->setStyleSheet("QPushButton{border: 1px solid black;"
                                    "border-radius: 20px;"
                                    "background-position: bottom center;"
                                    "background-color: rgba(255, 255, 255, 250);"
                                    "font-size: 17px;"
                                    "color: rgba(0, 0, 0, 200);}"
                                    "QPushButton:hover{border: 1px solid black;"
                                    "border-radius: 20px;"
                                    "background-position: bottom center;"
                                    "background-color: rgba(125, 125, 125, 250);"
                                    "font-size: 17px;"
                                    "color: rgba(250, 250, 250, 250);}"
                                    "QLabel{border: 1px solid black;"
                                    "background-position: bottom center;"
                                    "background-color: rgb(255, 255, 255);"
                                    "font-size: 15px;"
                                    "color: rgb(0, 0, 0);}"
                                    "QRadioButton{"
                                    "background-position: bottom center;"
                                    "font-size: 15px;"
                                    "color: rgb(255, 255, 255);}");

    QLabel *gameSizeLabel = new QLabel("Game Size", createGameWidget);
    gameSizeLabel->setGeometry(0, 0, 150, 45);
    gameSizeLabel->setStyleSheet("background-position: bottom center;"
                                 "background-color: rgb(255, 255, 255);"
                                 "font-size: 25px;"
                                 "color: rgb(0, 0, 0);");

    sz_nineteen->setGeometry(25, 50, 100, 20);
    sz_thirteen->setGeometry(25, 75, 100, 20);
    sz_nine->setGeometry(25, 100, 100, 20);
    ipRead->setGeometry(0, 150, 200, 40);
    portRead->setGeometry(0, 190, 200, 40);
    createBackButton->setGeometry(0, 250, 150, 40);

    connect(createGameButton, &QPushButton::clicked, this, &MultiplayerMenuWidget::createGamePushed);
    connect(findGameButton, &QPushButton::clicked, this, &MultiplayerMenuWidget::findGamePushed);

    connect(findBackButton, &QPushButton::clicked, this, &MultiplayerMenuWidget::hideFindGame);
    connect(createBackButton, &QPushButton::clicked, this, &MultiplayerMenuWidget::hideCreateGame);

    connect(exitButton, &QPushButton::clicked, this, &MultiplayerMenuWidget::exitButtonPushed);

    connect(findFindButton, &QPushButton::clicked, this, &MultiplayerMenuWidget::findGameButtonPushed);
}

MultiplayerMenuWidget::~MultiplayerMenuWidget()
{
    /*
        Delete MultiplayerMenuWidget, end socket and close, close server
    */
    if (socket != nullptr)
    {
        if (socket->state() == QTcpSocket::ConnectedState)
        {
            socket->disconnectFromHost();
        }
        else if (socket->state() == QTcpSocket::ConnectingState)
        {
            socket->abort();
        }
        socket->close();
        delete socket;
    }
    if (server != nullptr)
    {
        server->close();
        delete server;
    }
}

void MultiplayerMenuWidget::exitButtonPushed()
{
    /*
        Back to home
    */
    mainstack->setCurrentIndex(0);
}

void MultiplayerMenuWidget::createGamePushed()
{
    /*
        Create Game button pushed, setup QTcpServer, await connection
    */
    hideFindGame();
    if (server == nullptr)
    {
        server = new QTcpServer(createGameWidget);
    }
    server->listen(QHostAddress::Any);
    connect(server, &QTcpServer::newConnection, this, &MultiplayerMenuWidget::serverConnected);

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address : QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
            ipRead->setText(QString("IP Address: ") + address.toString());
    }
    portRead->setText(QString("Port: ") + QString::number(server->serverPort()));
    createGameWidget->show();
}

void MultiplayerMenuWidget::findGamePushed()
{
    /*
        Find Game button pushed, setup QTcpSocket, wait for user input
    */
    hideCreateGame();
    if (socket == nullptr)
    {
        socket = new QTcpSocket(findGameWidget);
    }
    findGameWidget->show();
}

void MultiplayerMenuWidget::findGameButtonPushed()
{
    /*
        Find Game button pushed, give parameters to QTcpSocket, connect
    */
    qDebug() << "Socket";
    socket->connectToHost(ipText->displayText(), quint16(portText->displayText().toInt()));
    //    connect socket if connected signal to socket connected
    connect(socket, &QTcpSocket::readyRead, this, &MultiplayerMenuWidget::socketRead);
}

void MultiplayerMenuWidget::hideCreateGame()
{
    /*
        Hide create game
    */
    if (server != nullptr)
    {
        server->close();
    }
    if (socket != nullptr)
    {
        socket->abort();
        socket->close();
    }
    createGameWidget->hide();
}

void MultiplayerMenuWidget::hideFindGame()
{
    /*
        Hide find game
    */
    if (socket != nullptr)
    {
        socket->abort();
        socket->close();
    }
    findGameWidget->hide();
}

void MultiplayerMenuWidget::socketRead()
{
    /*
        Read from socket, initialize game
    */
    if (!initialSent)
    {
        QByteArray datas = socket->readAll();
        QStringList list = QString(datas).split(" ", QString::SkipEmptyParts);
        enum size sz;

        if (list[0] == "19")
        {
            sz = nineteen;
        }
        else if (list[0] == "13")
        {
            sz = thirteen;
        }
        else if (list[0] == "9")
        {
            sz = nine;
        }
        else
        {
            throw(0);
        }
        if (list[1] == "black")
        {
            game = new GoGameWidget(this, mainstack, sz, human, humanTcp);
        }
        else if (list[1] == "white")
        {
            game = new GoGameWidget(this, mainstack, sz, humanTcp, human);
        }
        findGameWidget->hide();
        game->player_1->setSocket(socket);
        game->player_2->setSocket(socket);
        game->show();
        disconnect(socket, &QTcpSocket::readyRead, this, &MultiplayerMenuWidget::socketRead);
        if (list[1] == "black")
        {
            QMessageBox msgBox(backgroundWidget);
            msgBox.setText("Your Color is Black");
            msgBox.exec();
        }
        else if (list[1] == "white")
        {
            QMessageBox msgBox(backgroundWidget);
            msgBox.setText("Your Color is White");
            msgBox.exec();
        }
    }
}

void MultiplayerMenuWidget::serverConnected()
{
    /*
        Read from server, initialize game
        Send game parmeters to other socket
        Format: "(Game Size) (Other Game Stone)"
                Ex. "19 Black" means board is 19 and the other computer's game is stone 19
                    Stone color is given in random.
    */
    socket = server->nextPendingConnection();
    int randomInt = rand() % 2;
    stone selfType = noStone;
    enum size sz;
    QString str;
    if (sz_nineteen->isChecked())
    {
        str = QString("19 ");
        sz = nineteen;
    }
    else if (sz_thirteen->isChecked())
    {
        str = QString("13 ");
        sz = thirteen;
    }
    else if (sz_nine->isChecked())
    {
        str = QString("9 ");
        sz = nine;
    }
    else
    {
        throw(0);
    }

    if (randomInt)
    {
        selfType = black;
        game = new GoGameWidget(this, mainstack, sz, human, humanTcp);
    }
    else
    {
        selfType = white;
        game = new GoGameWidget(this, mainstack, sz, humanTcp, human);
    }
    game->player_1->setSocket(socket);
    game->player_2->setSocket(socket);

    str += (selfType == black ? "white" : "black");
    socket->write(QByteArray(str.toUtf8()));
    initialSent = true;
    game->show();
    QMessageBox msgBox(backgroundWidget);
    msgBox.setText(selfType == black ? "Your Color is Black" : "Your Color is White");
    msgBox.exec();
}
