#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gogamewidget.h"
#include "multiplayermenuwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    /*
        Main window class
    */
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void on_homeButton_clicked();

private slots:
    void on_menuButton_clicked();
    void on_gameButton_clicked();
    void on_backButton_clicked();
    void on_multiMenuButton_clicked();

private:
    Ui::MainWindow *ui;
    GoGameWidget* gameWgt;
    MultiplayerMenuWidget* multiMenu;
};
#endif // MAINWINDOW_H
