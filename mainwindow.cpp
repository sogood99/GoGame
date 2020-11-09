#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    // Initialize MainWindow
    ui->setupUi(this);
    setFixedSize(800,600);
    setCentralWidget(ui->mainStack);
    QPixmap bkgnd(":/img/homepageBackground.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    QLabel* label = ui->mainStack->findChild<QLabel*>("homeLabel");
    label->setAlignment(Qt::AlignCenter);
    label = ui->mainStack->findChild<QLabel*>("menuLabel");
    label->setAlignment(Qt::AlignCenter);
    label->setFont(QFont("Times", 100, QFont::Bold, false));
}

MainWindow::~MainWindow(){
    delete ui;
    delete gameWgt;
}

void MainWindow::on_menuButton_clicked(){
    /*
        Change QStackLayout page to menu
    */
    if (gameWgt == nullptr){
        ui->mainStack->setCurrentIndex(1);
        return;
    }else if (gameWgt->winColor != noStone){
        delete gameWgt;
        gameWgt = nullptr;
        ui->mainStack->setCurrentIndex(1);
        return;
    }
    QMessageBox askIfcontinue;
    QString boardString;
    if (gameWgt->board->boardSize == nine){
        boardString = QString(" 9x9");
    }else if (gameWgt->board->boardSize == nineteen){
        boardString = QString(" 19x19");
    }else if (gameWgt->board->boardSize == thirteen){
        boardString = QString(" 19x19");
    }else{
        throw (0);
    }
    askIfcontinue.setText(QString("Game Active:")+boardString);
    askIfcontinue.addButton("Continue", QMessageBox::AcceptRole);
    askIfcontinue.addButton("New Game", QMessageBox::RejectRole);
    int result = askIfcontinue.exec();
    if (result == 0){
        ui->mainStack->setCurrentIndex(2);
    }else if (result == 1){
        delete gameWgt;
        gameWgt = nullptr;
        ui->mainStack->setCurrentIndex(1);
    }else{
        throw (0);
    }
}

void MainWindow::on_gameButton_clicked(){
    /*
        Change QStackLayout page to game, create game
    */
    enum size sz = nineteen;
    QRadioButton* sz_nine = ui->mainStack->findChild<QRadioButton*>("szNine");
    QRadioButton* sz_thirteen = ui->mainStack->findChild<QRadioButton*>("szThirteen");
    QRadioButton* sz_nineteen = ui->mainStack->findChild<QRadioButton*>("szNineteen");
    if (sz_nine->isChecked()){
        sz = nine;
    }else if (sz_thirteen->isChecked()){
        sz = thirteen;
    }else if (sz_nineteen->isChecked()){
        sz = nineteen;
    }
    gameWgt = new GoGameWidget(this, ui->mainStack,sz);
    ui->mainStack->insertWidget(2, gameWgt);
    ui->mainStack->setCurrentIndex(2);
}

void MainWindow::on_backButton_clicked(){
    /*
        Back to home
    */
    ui->mainStack->setCurrentIndex(0);
}

void MainWindow::on_multiMenuButton_clicked(){
    /*
        Change QStackLayout page to multiplayermenu
    */
    multiMenu = new MultiplayerMenuWidget(this, ui->mainStack);
    ui->mainStack->insertWidget(3,multiMenu);
    ui->mainStack->setCurrentIndex(3);
}
