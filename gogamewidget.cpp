#include "gogamewidget.h"


GoGameWidget::GoGameWidget(QWidget *parent, QStackedWidget* mainstack, enum size sz, PlayerType player_1_type,
             PlayerType player_2_type) : QWidget(parent){
    /*
        Initialize GoGameWidget
    */
    GoGameWidget::parent = parent;
    GoGameWidget::mainstack = mainstack;
    board = new GoBoard(sz);
    GoGameWidget::player_1_type = player_1_type;
    GoGameWidget::player_2_type = player_2_type;
    if (player_1_type == human){
        player_1 = new HumanGoPlayer(black);
    }else if (player_1_type == computer_easy){
        player_1 = new ComputerGoPlayerEasy(black);
    }else if (player_1_type == humanTcp){
        player_1 = new HumanTcpGoPlayer(black);
    }
    if (player_2_type == human){
        player_2 = new HumanGoPlayer(white);
    }else if (player_2_type == computer_easy){
        player_2 = new ComputerGoPlayerEasy(white);
    }else if (player_2_type == humanTcp){
        player_2 = new HumanTcpGoPlayer(white);
    }else{
        throw(1);
    }
    captureCount.insert(std::map<stone, int>::value_type (black,0));
    captureCount.insert(std::map<stone, int>::value_type (white,0));
    backgroundWidget = new QWidget(this);
    backgroundWidget->setGeometry(0,0,800,600);
    backgroundWidget->setStyleSheet("background-color:grey");
    backgroundWidget->show();
    backgroundLabel = new QLabel(backgroundWidget);
    homeButton = new QPushButton(backgroundWidget);
    homeButton->setGeometry(600,50,100,50);
    homeButton->setText("Home");
    homeButton->setStyleSheet("border: 1px solid black;"
                              "border-radius: 20px;"
                              "background-position: bottom center;"
                              "background-color: rgba(255, 255, 255, 250);"
                              "font-size: 17px;"
                              "color: rgba(0, 0, 0, 200);");
    connect(homeButton, &QPushButton::clicked, this, &GoGameWidget::backhome);
    paintInit();
    parent->setWindowTitle("Go Game");
    for (int i = 0; i < sz; i++){
        stonesButtonVector.append(QVector<QPushButton*>());
        for (int j = 0; j < sz; j++){
            QPushButton* but = nullptr;
            if ((but = new QPushButton(backgroundWidget))){
                but->setStyleSheet("background-color: rgba(0,0,0,0)");
                but->setFlat(true);
                if (sz == nineteen){
                    but->setGeometry(12+28*j,12+28*i,25,25);
                }else if (sz == nine){
                    but->setGeometry(10+58*j, 10+58*i, 56, 56);
                }else if (sz == thirteen){
                    but->setGeometry(10+40*j,10+40*i, 38, 38);
                }
                but->show();
                stonesButtonVector[i].append(but);
                QObject::connect(but, &QPushButton::clicked, this, &GoGameWidget::parseInput);
            }else{
                throw(0);
            }
        }
    }
    moveLabel = new QLabel(backgroundWidget);
    if (moveLabel != nullptr){
        QFont newFont("Courier", 15, QFont::Bold, false);
        moveLabel->setGeometry(550,200,200,60);
        moveLabel->setFont(newFont);
        moveLabel->setAlignment(Qt::AlignCenter);
        updateLabel();
    }else{
        throw (0);
    }
    passButton = new QPushButton(backgroundWidget);
    if (passButton != nullptr){
        passButton->setText("Pass");
        passButton->setGeometry(600,300,100,50);
        QObject::connect(passButton, &QPushButton::clicked, this, &GoGameWidget::parseInput);
        passButton->setStyleSheet("border: 1px solid black;"
                                  "border-radius: 20px;"
                                  "background-position: bottom center;"
                                  "background-color: rgba(255, 255, 255, 250);"
                                  "font-size: 17px;"
                                  "color: rgba(0, 0, 0, 200);");
    }else{
        throw (0);
    }
    resignButton = new QPushButton(backgroundWidget);
    if (resignButton != nullptr){
        resignButton->setText("Resign");
        resignButton->setGeometry(600,400,100,50);
        QObject::connect(resignButton, &QPushButton::clicked, this, &GoGameWidget::parseInput);
        resignButton->setStyleSheet("border: 1px solid black;"
                                  "border-radius: 20px;"
                                  "background-position: bottom center;"
                                  "background-color: rgba(255, 255, 255, 250);"
                                  "font-size: 17px;"
                                  "color: rgba(0, 0, 0, 200);");
    }else{
        throw (0);
    }
    connect(player_1,&GoPlayer::moveSeted, this, &GoGameWidget::getInput);
    connect(player_2,&GoPlayer::moveSeted, this, &GoGameWidget::getInput);
    finalBoard = QVector<QVector<stone>>(sz, QVector<stone>(sz,noStone));
    placeStoneAudio = new QSound(":/sound/PlaceStoneAudio.wav", backgroundWidget);
    captureAudio = new QSound(":/sound/CaptureAudio.wav", backgroundWidget);
}

GoGameWidget::~GoGameWidget(){
    delete board;
    delete player_1;
    delete player_2;
    delete deadStones;
}

void GoGameWidget::placeMove(Pos p){
    /*
        Place stone on goboard
    */
    boolInt bi = board->placeStone(p, currentMove);
    if (bi.boolean){
        captureCount[currentMove] += bi.integer;
        currentMove = otherStone(currentMove);
        moveNum += 1;
        updateLabel();
    }
}

void GoGameWidget::updateLabel(){
    /*
        Update Label
    */
    moveLabel->setText(currentMove == black? "Current Move\nBlack": "Current Move\nWhite");
    moveLabel->setStyleSheet((currentMove== black? "color: black": "color: white") );
}

void GoGameWidget::parseInput(){
    /*
        Get input from buttons in the widget, then send to player
    */
    GoPlayer* currentPlayer = nullptr;
    if (currentMove == black){
        if (player_1_type != human){
            return;
        }else{
            currentPlayer = player_1;
        }
    }else{
        if (player_2_type != human){
            return;
        }else{
            currentPlayer = player_2;
        }
    }
    const QObject* button = QObject::sender();

    if (button == resignButton){
        class GoMove mv(resign,Pos(-1,-1));
        currentPlayer->setMove(mv);
        return;
    }
    if (button == passButton){
        class GoMove mv(pass,Pos(-1,-1));
        currentPlayer->setMove(mv);
        return;
    }
    int n = board->boardSize;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (stonesButtonVector[i][j] == button){
                class GoMove mv(place, Pos(i,j));
                currentPlayer->setMove(mv);
            }
        }
    }
}

void GoGameWidget::getInput(GoMove mv){
    /*
        Get move from GoPlayer class
    */
    if (mv.isSet){
        if (mv.mt == place){
            boolInt canPlace = board->placeStone(mv.pos, currentMove);
            if (canPlace.boolean){
                passCount = 0;
                updateBoard();
                placeStoneAudio->play();
                if (canPlace.integer){
                    captureCount[currentMove] += canPlace.integer;
                    captureAudio->play();
                }
                currentMove = otherStone(currentMove);
                updateLabel();
            }
        }else if (mv.mt == resign){
            endGame(resign);
        }else if (mv.mt == pass){
            passCount ++;
            if (passCount >= 2){
                endGame(pass);
            }else{
                currentMove = otherStone(currentMove);
                updateLabel();
            }
        }
    }
}

void GoGameWidget::updateBoard(bool deadStonePhase){
    /*
        Update the drawing of board using painter
        If in deadstone phase, color deadstone with some transparency
    */
    QPixmap pixmap = paintInit();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    int n = board->boardSize;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            stone mv = board->currentBoard[i][j];
            if(deadStonePhase && (*deadStones)[i][j] == isDead){
                if (mv == black){
                    painter.setBrush(QColor(0, 0, 0, 100));
                }else if (mv == white){
                    painter.setBrush(QColor(255, 255, 255, 100));
                }
            }else if (mv != noStone){
                painter.setBrush(QBrush(QColor(mv == black ? "black": "white")));
            }
            if (mv != noStone){
                painter.drawEllipse(stonesButtonVector[i][j]->geometry());
            }
        }
    }
    backgroundLabel->setPixmap(pixmap);
}

QPixmap GoGameWidget::paintInit(){
    /*
        Paint empty board, used in update board, returns the pen of painter
    */
    QPixmap pixmap(600,600);
    pixmap.fill(QColor("transparent"));
    QPainter painter(&pixmap);
    if (board->boardSize == nineteen){
        painter.setBrush(QBrush(QColor("orange")));
        painter.drawRect(10, 10, 532, 532);
        painter.setBrush(QBrush(QColor("black")));
        for (int i = 0; i < 19; i++){
            painter.drawLine(10+14*(2*i+1), 24, 10+14*(2*i+1), 528);
        }
        for (int j = 0; j < 19; j++){
            painter.drawLine(24,10+14*(2*j+1),528,10+14*(2*j+1));
        }
        painter.setRenderHint(QPainter::Antialiasing);
        int dotsOnBoard[] = {3,9,15};

        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                painter.drawEllipse(10+14*(2*dotsOnBoard[i]+1)-3,10+14*(2*dotsOnBoard[j]+1)-3,6,6);
            }
        }
    }else if (board->boardSize == nine){
        painter.setBrush(QBrush(QColor("orange")));
        painter.drawRect(7, 7, 528, 528);
        painter.setBrush(QBrush(QColor("black")));
        for (int i = 0; i < 9; i++){
            painter.drawLine(10+29*(2*i+1), 39, 10+29*(2*i+1), 503);
        }
        for (int j = 0; j < 9; j++){
            painter.drawLine(39, 10+29*(2*j+1), 503,10+29*(2*j+1));
        }
        painter.setRenderHint(QPainter::Antialiasing);

        int dotsOnBoard[] = {2,6};

        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 2; j++){
                painter.drawEllipse(10+29*(2*dotsOnBoard[i]+1)-5,10+29*(2*dotsOnBoard[j]+1)-5,10,10);
            }
        }
        painter.drawEllipse(10+29*(2*4+1)-5,10+29*(2*4+1)-5,10,10);
    }else if (board->boardSize == thirteen){
        painter.setBrush(QBrush(QColor("orange")));
        painter.drawRect(8, 8, 522, 522);
        painter.setBrush(QBrush(QColor("black")));
        for (int i = 0; i < 13; i++){
            painter.drawLine(10+20*(2*i+1), 30, 10+20*(2*i+1), 510);
        }
        for (int j = 0; j < 13; j++){
            painter.drawLine(30,10+20*(2*j+1), 510, 10+20*(2*j+1));
        }
        painter.setRenderHint(QPainter::Antialiasing);

        int dotsOnBoard[] = {3, 9};

        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 2; j++){
                painter.drawEllipse(10+20*(2*dotsOnBoard[i]+1)-3,10+20*(2*dotsOnBoard[j]+1)-3,6,6);
            }
        }
        painter.drawEllipse(10+20*(2*6+1)-3,10+20*(2*6+1)-3,6,6);
    }
    backgroundLabel->setPixmap(pixmap);
    return pixmap;
}

void GoGameWidget::backhome(){
    /*
        Return back to home, used by home button
    */
    mainstack->setCurrentIndex(0);
}


void GoGameWidget::endGame(moveType mt){
    /*
        Function for during the end game (pass, resign, etc)
    */
    if (mt == pass){
        for (int i = 0; i < board->boardSize; i++){
            for (int j = 0; j < board->boardSize; j++){
                QPushButton* but = stonesButtonVector[i][j];
                disconnect(but, &QPushButton::clicked, this, &GoGameWidget::parseInput);
            }
        }
        disconnect(resignButton, &QPushButton::clicked, this, &GoGameWidget::parseInput);
        disconnect(passButton, &QPushButton::clicked, this, &GoGameWidget::parseInput);
        disconnect(player_1,&GoPlayer::moveSeted, this, &GoGameWidget::getInput);
        disconnect(player_2,&GoPlayer::moveSeted, this, &GoGameWidget::getInput);
        QStatusBar msgBox;
        msgBox.showMessage("Stone Removal Phase");
//        msgBox.addButton(QString("Manual"),QMessageBox::AcceptRole);
//        msgBox.addButton(QString("Automatic"),QMessageBox::RejectRole);
        moveLabel->setText("Remove\nDead Stones");
//        int result = msgBox.exec();
        deadStones = new QVector<QVector<DeadStone>>(board->boardSize, QVector<DeadStone>(board->boardSize, notDead));
//        if (result == 0){
            removeDeadStones();
//        }else if (result == 1){
//            automaticallyRemoveDeadStones();
//        }
    }else if(mt == resign){
        winColor = otherStone(currentMove);
        moveLabel->setText(QString(otherStone(currentMove) == black? "Black": "White")+QString(" Wins\nBy Resignation"));
        moveLabel->setStyleSheet("color: yellow");
        for (int i = 0; i < board->boardSize; i++){
            for (int j = 0; j < board->boardSize; j++){
                QPushButton* but = stonesButtonVector[i][j];
                disconnect(but, &QPushButton::clicked, this, &GoGameWidget::parseInput);
            }
        }
        disconnect(resignButton, &QPushButton::clicked, this, &GoGameWidget::parseInput);
        disconnect(passButton, &QPushButton::clicked, this, &GoGameWidget::parseInput);
        QMessageBox msgBox;
        msgBox.setText(currentMove == black ? "Black Resigns": "White Resigns");
        msgBox.exec();
    }
}

void GoGameWidget::removeDeadStones(){
    /*
        Manually remove dead stones
    */
    int n = board->boardSize;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            QPushButton* but = stonesButtonVector[i][j];
            connect(but, &QPushButton::clicked, this, &GoGameWidget::parseInputDeadStone);
        }
    }
    connect(player_1,&GoPlayer::moveSeted, this, &GoGameWidget::deadStoneSlot);
    connect(player_2,&GoPlayer::moveSeted, this, &GoGameWidget::deadStoneSlot);
    endStoneRemoval = new QPushButton(backgroundWidget);
    endStoneRemoval->setGeometry(590,500,120,50);
    endStoneRemoval->setText("End Removal");
    endStoneRemoval->setStyleSheet("border: 1px solid black;"
                              "border-radius: 20px;"
                              "background-position: bottom center;"
                              "background-color: rgba(255, 200, 200, 250);"
                              "font-size: 17px;"
                              "color: rgba(0, 0, 0, 200);");
    endStoneRemoval->show();
    connect(endStoneRemoval, &QPushButton::clicked, this, &GoGameWidget::endRemoveDeadStones);
    connect(player_1, &GoPlayer::hideEndButton, this, &GoGameWidget::hideEndButton);
    connect(player_2, &GoPlayer::hideEndButton, this, &GoGameWidget::hideEndButton);
    connect(player_1, &GoPlayer::beginDeadStoneMessage, this, &GoGameWidget::endRemoveDeadStones);
    connect(player_2, &GoPlayer::beginDeadStoneMessage, this, &GoGameWidget::endRemoveDeadStones);
}

void GoGameWidget::hideEndButton(){
    /*
        Hide end button
    */
    if (endStoneRemoval != nullptr){
        endStoneRemoval->hide();
    }
}

void GoGameWidget::parseInputDeadStone(){
    /*
        Parse input for dead stones, similar to parse input but specifically designed for dead stone phase.
    */
    GoPlayer* currentPlayer = nullptr;
    if (player_1_type == human){
        currentPlayer = player_1;;
    }else if (player_2_type == human){
        currentPlayer = player_2;
    }else{
        throw(0);
    }
    const QObject* button = QObject::sender();
    int n = board->boardSize;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (stonesButtonVector[i][j] == button){
                class GoMove mv(place, Pos(i,j));
                currentPlayer->setMove(mv);
            }
        }
    }
}

void GoGameWidget::deadStoneSlot(GoMove mv){
    /*
        Slot for connecting to parse input dead stone,
        Used to set stones on board to dead.
    */
    if (mv.isSet){
        if (mv.mt == place){
            int i = mv.pos.position[0];
            int j = mv.pos.position[1];
            if (board->currentBoard[i][j] != noStone){
                if ((*deadStones)[i][j] != isDead){
                    (*deadStones)[i][j] = isDead;
                    updateBoard(true);
                }else if ((*deadStones)[i][j] == isDead){
                    (*deadStones)[i][j] = notDead;
                    updateBoard(true);
                }
            }
        }
    }
}

void GoGameWidget::showFinalBoard(){
    /*
        Show final board
    */
    QPixmap pixmap = paintInit();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    int n = board->boardSize;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            stone mv = finalBoard[i][j];
            if ((*deadStones)[i][j] == isDead){
                if (board->currentBoard[i][j] == black){
                    painter.setBrush(QColor(0, 0, 0, 100));
                }else if (board->currentBoard[i][j] == white){
                    painter.setBrush(QColor(255, 255, 255, 100));
                }
                painter.drawEllipse(stonesButtonVector[i][j]->geometry());
            }
            if (board->currentBoard[i][j] != mv){
                if (mv != noStone){
                    if (mv == black){
                        painter.setBrush(QColor(0, 0, 0));
                    }else if (mv == white){
                        painter.setBrush(QColor(255,255,255));
                    }
                    QRect geo = stonesButtonVector[i][j]->geometry();
                    geo.setWidth(geo.width()/2);
                    geo.setHeight(geo.height()/2);
                    geo.moveTo(geo.x()+geo.width()/2,geo.y()+geo.height()/2);
                    painter.drawRect(geo);
                }
            }else{
                if (mv != noStone){
                    painter.setBrush(QBrush(QColor(mv == black ? "black": "white")));
                    painter.drawEllipse(stonesButtonVector[i][j]->geometry());
                }
            }
        }
    }
    backgroundLabel->setPixmap(pixmap);
}

void GoGameWidget::computeFinalScore(){
    /*
        Compute final score
    */
    float komi = 7.5;
    float black_score = 0;
    float white_score = komi;
    for (int i = 0; i < board->boardSize; i++){
        for (int j = 0; j < board->boardSize; j++){
            if (finalBoard[i][j] == black){
                black_score ++;
            }else if (finalBoard[i][j] == white){
                white_score ++;
            }
        }
    }
    winColor = (black_score > white_score? black: white);
    moveLabel->setText(QString("Black Score: ")+QString::number(black_score)+QString("\nWhite Score: ")+QString::number(white_score));
    QMessageBox msgBox(backgroundWidget);
    msgBox.setText(black_score > white_score? "Black Wins" : "White Wins");
    msgBox.exec();
}

void GoGameWidget::endRemoveDeadStones(){
    /*
        End remove dead stone phase, used by end remove dead stone button
    */
    connect(player_1, &GoPlayer::deadStonesSetted, this, &GoGameWidget::getDeadStonesChoiceinput);
    connect(player_2, &GoPlayer::deadStonesSetted, this, &GoGameWidget::getDeadStonesChoiceinput);
    if (player_1_type == human && player_1->choice == notSet){
        player_1->setupRemoveDeadStones();
    }
    if (player_2_type == human && player_2->choice == notSet){
        player_2->setupRemoveDeadStones();
    }
}

void GoGameWidget::getDeadStonesChoiceinput(){
    /*
        Used to get input from player on if remove dead stone
        Choices are {accept, reject, continuePlay}
    */
    if (player_1->choice != notSet && player_2->choice != notSet){
        if (player_1->choice == continuePlay || player_2->choice == continuePlay){
            QMessageBox msgbx;
            msgbx.setText("Game Continue");
            msgbx.exec();
            disconnect(endStoneRemoval, &QPushButton::clicked, this, &GoGameWidget::endRemoveDeadStones);
            delete endStoneRemoval;
            endStoneRemoval = nullptr;
            int n = board->boardSize;
            for (int i = 0; i < n; i++){
                for (int j = 0; j < n; j++){
                    QPushButton* but = stonesButtonVector[i][j];
                    disconnect(but, &QPushButton::clicked, this, &GoGameWidget::parseInputDeadStone);
                    connect(but, &QPushButton::clicked, this, &GoGameWidget::parseInput);
                }
            }
            player_1->endRemoveDeadStones();
            player_2->endRemoveDeadStones();
            connect(resignButton, &QPushButton::clicked, this, &GoGameWidget::parseInput);
            connect(passButton, &QPushButton::clicked, this, &GoGameWidget::parseInput);
            disconnect(player_1, &GoPlayer::moveSeted, this, &GoGameWidget::deadStoneSlot);
            disconnect(player_2, &GoPlayer::moveSeted, this, &GoGameWidget::deadStoneSlot);
            connect(player_1, &GoPlayer::moveSeted, this, &GoGameWidget::getInput);
            connect(player_2, &GoPlayer::moveSeted, this, &GoGameWidget::getInput);
            currentMove = otherStone(currentMove);
            updateBoard();
            updateLabel();
        }else if (player_1->choice == reject || player_2->choice == reject){
            endStoneRemoval->show();
            player_1->endRemoveDeadStones();
            player_2->endRemoveDeadStones();
        }else if (player_1->choice == accept && player_2->choice == accept){
            finalBoard = board->floodFill(*deadStones);
            showFinalBoard();
            computeFinalScore();
            delete endStoneRemoval;
            endStoneRemoval = nullptr;
            for (int i = 0; i < board->boardSize; i++){
                for (int j = 0; j < board->boardSize; j++){
                    QPushButton*  but = stonesButtonVector[i][j];
                    disconnect(but, &QPushButton::clicked, this, &GoGameWidget::parseInputDeadStone);
                }
            }
        }
        player_1->choice = notSet;
        player_2->choice = notSet;
    }

}

void GoGameWidget::automaticallyRemoveDeadStones(){
    /*
        Use monte carlo method to detect if stones are dead
        !UNFINISHED!
    */
    qDebug()<<"Automatically Removed Dead Stones\n";
}
