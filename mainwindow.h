#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QDebug>

const int WIDTH = 1200;
const int HEIGHT = 800;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMenuBar* topMenuBar;

    QMenu* fileMenu;
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;

    QMenu* editMenu;
    QAction* copyAction;
    QAction* cutAction;
    QAction* pasteAction;
    QAction* deleteAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* findAction;
    QAction* replaceAction;

    QAction* complieAction;
    QAction* runAction;

    void CreateTopMenuBar();
};

#endif // MAINWINDOW_H
