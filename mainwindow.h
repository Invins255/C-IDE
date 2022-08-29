#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include "codeeditor.h"
#include "compiler.h"
#include "filesystem.h"

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

    QVBoxLayout* vlayout;

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

    QTabWidget* editTab;
    QVector<CodeEditor*> editors;
    CodeEditor* lastEditor;
    CodeEditor* currentEditor;

    QTabWidget* tabwidget;
    QTextEdit* logOutput;
    QTextEdit* appOutPut;
    QTextEdit* compilerOutPut;

    void CreateTopMenuBar();
    void CreateShortcutBar();
    void CreateEditTab();
    void CreateCodeEdit(const QString& tabName);
    void CreateInfoBrowser();
    void CreateVLayout();

    void ConnectEditAction(CodeEditor* editor);
    void DisconnectEditAction(CodeEditor* editor);

    QString GetCurrentFilePath();
private slots:
    void TranslateEditTab();
    void CloseTab(int index);
};

#endif // MAINWINDOW_H
