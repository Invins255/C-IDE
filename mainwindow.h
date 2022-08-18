#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QDebug>
#include "codeeditor.h"

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
    QMenuBar* shortcutBar;

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

    QMenu* buildMenu;
    QAction* compileAction;
    QAction* runAction;

    CodeEditor* edit;

    void CreateTopMenuBar();
    void CreateShortcutBar();
    void CreateCodeEdit();
};

#endif // MAINWINDOW_H
