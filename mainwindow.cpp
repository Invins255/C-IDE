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
    CreateShortcutBar();
    CreateCodeEdit();
    CreateInfoBrowser();

    /*Menu bar slots*/
    connect(copyAction,&QAction::triggered,this->edit,&CodeEditor::copy);
    connect(cutAction,&QAction::triggered,this->edit,&CodeEditor::cut);
    connect(pasteAction,&QAction::triggered,this->edit,&CodeEditor::paste);
    connect(deleteAction,&QAction::triggered,this->edit,&CodeEditor::deleteText);
    connect(undoAction,&QAction::triggered,this->edit,&CodeEditor::undo);
    connect(redoAction,&QAction::triggered,this->edit,&CodeEditor::redo);
    connect(findAction,&QAction::triggered,this->edit,&CodeEditor::CreateFindDialog);
    connect(replaceAction,&QAction::triggered,this->edit,&CodeEditor::CreateReplaceDialog);

    /*Compile and Run slots*/
    connect(&(Compiler::getInstance()),&Compiler::startCompiling,this,[this](){
        logOutput->append("Compiling...");
    });
    connect(&(Compiler::getInstance()),&Compiler::startRunning,this,[this](){
        logOutput->append("Program is running...");
    });
    connect(compileAction,&QAction::triggered,this,[this](){
        Compiler::getInstance().Compile(FileSystem::getInstance().currentFilePath);
    });
    connect(runAction,&QAction::triggered,this,[this](){
        Compiler::getInstance().Run(Compiler::RemoveExtension(FileSystem::getInstance().currentFilePath)+".exe");
    });
    connect(&(Compiler::getInstance()),&Compiler::compileFinished,this,[this](){
        logOutput->append("Compiled.");
        compilerOutPut->setText(Compiler::getInstance().StandardError());
    });
    connect(&(Compiler::getInstance()),&Compiler::runFinished,this,[this](){
        logOutput->append("Program run ends.");
        appOutPut->setText(Compiler::getInstance().StandardOutput());
    });

    /*FileSystem slots*/
    connect(newAction,&QAction::triggered,this,[this](){
        FileSystem::getInstance().NewFile();
        edit->clear();
        FileSystem::getInstance().content = edit->toPlainText();
    });
    connect(openAction,&QAction::triggered,this,[this](){
        FileSystem::getInstance().Open();
        edit->clear();
        edit->appendPlainText(FileSystem::getInstance().content);
    });
    connect(saveAction,&QAction::triggered,this,[this](){
        FileSystem::getInstance().content = edit->toPlainText();
        FileSystem::getInstance().Save();
    });
    connect(saveAsAction,&QAction::triggered,this,[this](){
        FileSystem::getInstance().content = edit->toPlainText();
        FileSystem::getInstance().SaveAs();
    });

    /*Shortcut*/
    newAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    openAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    saveAsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    copyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    pasteAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    cutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    undoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    redoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
    findAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    replaceAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F));
    deleteAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    compileAction->setShortcut(QKeySequence(Qt::Key_F5));
    runAction->setShortcut(QKeySequence(Qt::Key_F6));

}

MainWindow::~MainWindow()
{
    delete ui;

    delete topMenuBar;
    delete shortcutBar;

    delete fileMenu;
    delete newAction;
    delete openAction;
    delete saveAction;
    delete saveAsAction;

    delete editMenu;
    delete copyAction;
    delete cutAction;
    delete pasteAction;
    delete deleteAction;
    delete undoAction;
    delete redoAction;
    delete findAction;
    delete replaceAction;

    delete buildMenu;
    delete compileAction;
    delete runAction;

    delete edit;
}

void MainWindow::CreateTopMenuBar(){
    topMenuBar = new QMenuBar(this);
    topMenuBar->setGeometry(0,0,WIDTH,topMenuBar->height());

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

    /*Build menu*/
    buildMenu = new QMenu("Build",this);
    compileAction = new QAction("Compile",this);
    runAction = new QAction("Run",this);
    buildMenu->addAction(compileAction);
    buildMenu->addAction(runAction);
    topMenuBar->addMenu(buildMenu);

    /*Add Icon*/
    newAction->setIcon(QIcon(":/res/Resources/icon/new_file.png"));
    openAction->setIcon(QIcon(":/res/Resources/icon/open_file.png"));
    saveAction->setIcon(QIcon(":/res/Resources/icon/save_file.png"));
    saveAsAction->setIcon(QIcon(":/res/Resources/icon/save_all.png"));
    copyAction->setIcon(QIcon(":/res/Resources/icon/copy.png"));
    pasteAction->setIcon(QIcon(":/res/Resources/icon/paste.png"));
    cutAction->setIcon(QIcon(":/res/Resources/icon/cut.png"));
    deleteAction->setIcon(QIcon(":/res/Resources/icon/delete.png"));
    undoAction->setIcon(QIcon(":/res/Resources/icon/undo.png"));
    redoAction->setIcon(QIcon(":/res/Resources/icon/redo.png"));
    compileAction->setIcon(QIcon(":/res/Resources/icon/build.png"));
    runAction->setIcon(QIcon(":/res/Resources/icon/run.png"));
    findAction->setIcon(QIcon(":/res/Resources/icon/find.png"));
    replaceAction->setIcon(QIcon(":/res/Resources/icon/replace.png"));
}

void MainWindow::CreateShortcutBar(){
    shortcutBar = new QMenuBar(this);
    shortcutBar->setGeometry(0,topMenuBar->height(),WIDTH,shortcutBar->height());

    shortcutBar->addAction(newAction);
    shortcutBar->addAction(openAction);
    shortcutBar->addAction(saveAction);
    shortcutBar->addAction(saveAsAction);
    shortcutBar->addAction(copyAction);
    shortcutBar->addAction(cutAction);
    shortcutBar->addAction(pasteAction);
    shortcutBar->addAction(deleteAction);
    shortcutBar->addAction(undoAction);
    shortcutBar->addAction(redoAction);
    shortcutBar->addAction(findAction);
    shortcutBar->addAction(replaceAction);
    shortcutBar->addAction(compileAction);
    shortcutBar->addAction(runAction);
}

void MainWindow::CreateCodeEdit(){
    edit=new CodeEditor(this);
    edit->setGeometry(100,80,1100,500);
}

void MainWindow::CreateInfoBrowser(){
    tabwidget=new QTabWidget(this);
    tabwidget->setGeometry(100,580,1100,200);

    logOutput=new QTextEdit(this);
    logOutput->setReadOnly(true);
    logOutput->setFontPointSize(11);
    logOutput->setFontWeight(QFont::Medium);
    tabwidget->addTab(logOutput,"Information");

    appOutPut=new QTextEdit(this);
    appOutPut->setReadOnly(true);
    appOutPut->setFontPointSize(11);
    appOutPut->setFontWeight(QFont::Medium);
    tabwidget->addTab(appOutPut,"Application Output");

    compilerOutPut=new QTextEdit(this);
    compilerOutPut->setFontPointSize(11);
    compilerOutPut->setFontWeight(QFont::Medium);
    compilerOutPut->setReadOnly(true);
    tabwidget->addTab(compilerOutPut,"Compiler Output");
}


