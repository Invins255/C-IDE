#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(WIDTH,HEIGHT);
    setWindowTitle("Simple C IDE");

    CreateTopMenuBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateTopMenuBar(){
    topMenuBar = new QMenuBar(this);
    topMenuBar->resize(WIDTH,topMenuBar->height());

    /*File menu*/
    fileMenu = new QMenu("File",this);
    newAction = new QAction("New",this);
    openAction = new QAction("Open",this);
    saveAction = new QAction("Save",this);
    saveAsAction = new QAction("Save as",this);
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    topMenuBar->addMenu(fileMenu);

    /*Edit menu*/
    editMenu = new QMenu("Edit",this);
    copyAction = new QAction("Copy",this);
    cutAction = new QAction("Cut",this);
    pasteAction = new QAction("Paste",this);
    deleteAction = new QAction("delete",this);
    undoAction = new QAction("Undo",this);
    redoAction = new QAction("Redo",this);
    findAction = new QAction("Find",this);
    replaceAction = new QAction("Replace",this);
    editMenu->addAction(copyAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);
    editMenu->addSeparator();
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(replaceAction);
    topMenuBar->addMenu(editMenu);

    /*Compile and run*/
    complieAction = new QAction("Complie",this);
    runAction = new QAction("Run",this);
    topMenuBar->addAction(complieAction);
    topMenuBar->addAction(runAction);

    //TODO: Connect action slot
    //example: connect(action,&QAction::triggered,this,&actionFunc);
    //TODO: Set hot key
    //TODO: Set Icon
}

