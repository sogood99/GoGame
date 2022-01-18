#ifndef GOGAMEWIDGET_H
#define GOGAMEWIDGET_H

#include "goboard.h"
#include "goplayer.h"
#include <QMessageBox>
#include <QPainter>
#include <QStackedWidget>

enum PlayerType
{
    /*
        Enum for player types
    */
    human = 0,
    humanTcp = 1,
    computer_easy = 2
};

class GoGameWidget : public QWidget
{
    /*
        QWidget child for containing two players and a board
    */
    Q_OBJECT
public:
    GoBoard *board = nullptr;
    GoPlayer *player_1 = nullptr;
    GoPlayer *player_2 = nullptr;
    int moveNum = 0;
    stone currentMove = black;
    std::map<stone, int> captureCount;
    std::map<stone, int> score;
    stone winColor = noStone;

public:
    GoGameWidget(QWidget *parent = nullptr, QStackedWidget *mainstack = nullptr, enum size sz = nineteen, PlayerType player_1_type = human,
                 PlayerType player_2_type = human);
    ~GoGameWidget();
signals:

public slots:
    void parseInput();
    void getInput(GoMove mv);
    void backhome();
    void parseInputDeadStone();
    void deadStoneSlot(GoMove mv);
    void endRemoveDeadStones();
    void getDeadStonesChoiceinput();
    void hideEndButton();

protected:
    QWidget *parent = nullptr;
    QStackedWidget *mainstack = nullptr;
    QVector<QVector<stone>> finalBoard;
    void computeFinalScore();
    void homePressed();
    void removeDeadStones();
    void automaticallyRemoveDeadStones();
    void determineScore();
    bool checkIfEnd();
    void endGame(moveType mt);
    QPixmap paintInit();
    void updateBoard(bool deadStonePhase = false);
    void showFinalBoard();
    void placeMove(Pos p);
    void updateLabel();
    PlayerType player_1_type;
    PlayerType player_2_type;
    QVector<QVector<QPushButton *>> stonesButtonVector;
    QVector<QVector<DeadStone>> *deadStones = nullptr;
    QWidget *backgroundWidget = nullptr;
    QLabel *moveLabel = nullptr;
    QLabel *backgroundLabel = nullptr;
    QPushButton *resignButton = nullptr;
    QPushButton *passButton = nullptr;
    QPushButton *homeButton = nullptr;
    QPushButton *endStoneRemoval = nullptr;
    int passCount = 0;
    QSound *placeStoneAudio = nullptr;
    QSound *captureAudio = nullptr;
};

#endif // GOGAMEWIDGET_H
