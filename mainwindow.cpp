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
    CreateEditTab();
    CreateInfoBrowser();
    CreateVLayout();

    /*文件系统添加新文件时，增加编辑栏*/
    connect(&(FileSystem::getInstance()),&FileSystem::addFile,this,[this](const QString& fileName){
        CreateCodeEdit(fileName);

        if(currentEditor==nullptr){
            currentEditor=editors[0];
            ConnectEditAction(currentEditor);
        }
    });
    /*修改编辑相关槽函数绑定*/
    connect(editTab,&QTabWidget::currentChanged,this,&MainWindow::TranslateEditTab);

    /*Compile and Run slots*/
    connect(&(Compiler::getInstance()),&Compiler::startCompiling,this,[this](){
        logOutput->append(GetCurrentFilePath() + " is compiling...");
    });
    connect(&(Compiler::getInstance()),&Compiler::startRunning,this,[this](){
        logOutput->append(Compiler::RemoveExtension(GetCurrentFilePath())+".exe" + " is running...");
    });
    connect(compileAction,&QAction::triggered,this,[this](){
        if(editTab->count()==0)
            return;
        Compiler::getInstance().Compile(GetCurrentFilePath());
    });
    connect(runAction,&QAction::triggered,this,[this](){
        if(editTab->count()==0)
            return;
        Compiler::getInstance().Compile(GetCurrentFilePath());
        Compiler::getInstance().Run(Compiler::RemoveExtension(GetCurrentFilePath())+".exe");
    });
    connect(&(Compiler::getInstance()),&Compiler::compileFinished,this,[this](){
        logOutput->append(GetCurrentFilePath() + " is compiled.");
        compilerOutPut->clear();
        compilerOutPut->appendPlainText((Compiler::getInstance().StandardError()));
    });
    connect(&(Compiler::getInstance()),&Compiler::runFinished,this,[this](){
        logOutput->append(Compiler::RemoveExtension(GetCurrentFilePath())+".exe" + " ends.");
        appOutPut->setText(Compiler::getInstance().StandardOutput());
    });


    /*FileSystem slots*/
    connect(newAction,&QAction::triggered,this,[this](){
        FileSystem::getInstance().NewFile();
    });
    connect(openAction,&QAction::triggered,this,[this](){
        FileSystem::getInstance().Open();
        if(!FileSystem::getInstance().files.isEmpty())
            editors.last()->appendPlainText(FileSystem::getInstance().files.last().content);
    });
    connect(saveAction,&QAction::triggered,this,[this](){
        if(editTab->count()==0)
            return;
        int index = editTab->currentIndex();
        FileSystem::getInstance().files[index].content = editors[index]->toPlainText();
        FileSystem::getInstance().Save(index);
    });
    connect(saveAsAction,&QAction::triggered,this,[this](){
        if(editTab->count()==0)
            return;
        int index = editTab->currentIndex();
        FileSystem::getInstance().files[index].content = editors[index]->toPlainText();
        FileSystem::getInstance().SaveAs(index);
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

    newAction->setStatusTip("New");
    openAction->setStatusTip("Open");
    saveAction->setStatusTip("Save");
    saveAsAction->setStatusTip("Save as");
    copyAction->setStatusTip("Copy");
    pasteAction->setStatusTip("Paste");
    cutAction->setStatusTip("Cut");
    undoAction->setStatusTip("Undo");
    redoAction->setStatusTip("Redo");
    findAction->setStatusTip("Find");
    replaceAction->setStatusTip("Replace");
    deleteAction->setStatusTip("Delete");
    compileAction->setStatusTip("Compile");
    runAction->setStatusTip("Run");

}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::CreateEditTab(){
    editTab=new QTabWidget(this);
    editTab->setGeometry(100,60,1100,520);
    editTab->setTabsClosable(true);
    connect(editTab,&QTabWidget::tabCloseRequested,this,&MainWindow::CloseTab);
}

void MainWindow::CreateCodeEdit(const QString& tabName){
    CodeEditor* editor = new CodeEditor();
    editors.append(editor);
    editTab->addTab(editor,tabName);
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

    compilerOutPut = new InfoBrowser(this);
    compilerOutPut->setReadOnly(true);
    tabwidget->addTab(compilerOutPut,"Compiler Output");
}

void MainWindow::CreateVLayout(){
    vlayout = new QVBoxLayout();
    vlayout->addWidget(topMenuBar);
    vlayout->addWidget(shortcutBar);
    vlayout->addWidget(editTab);
    vlayout->addWidget(tabwidget);
    vlayout->setMargin(0);
    vlayout->setSpacing(0);
    vlayout->setStretchFactor(editTab,2);
    vlayout->setStretchFactor(tabwidget,1);
    ui->centralWidget->setLayout(vlayout);
}

void MainWindow::ConnectEditAction(CodeEditor* editor){
    connect(copyAction,&QAction::triggered,editor,&CodeEditor::copy);
    connect(cutAction,&QAction::triggered,editor,&CodeEditor::cut);
    connect(pasteAction,&QAction::triggered,editor,&CodeEditor::paste);
    connect(deleteAction,&QAction::triggered,editor,&CodeEditor::deleteText);
    connect(undoAction,&QAction::triggered,editor,&CodeEditor::undo);
    connect(redoAction,&QAction::triggered,editor,&CodeEditor::redo);
    connect(findAction,&QAction::triggered,editor,&CodeEditor::CreateFindDialog);
    connect(replaceAction,&QAction::triggered,editor,&CodeEditor::CreateReplaceDialog);
}

void MainWindow::DisconnectEditAction(CodeEditor* editor){
    disconnect(copyAction,&QAction::triggered,editor,&CodeEditor::copy);
    disconnect(cutAction,&QAction::triggered,editor,&CodeEditor::cut);
    disconnect(pasteAction,&QAction::triggered,editor,&CodeEditor::paste);
    disconnect(deleteAction,&QAction::triggered,editor,&CodeEditor::deleteText);
    disconnect(undoAction,&QAction::triggered,editor,&CodeEditor::undo);
    disconnect(redoAction,&QAction::triggered,editor,&CodeEditor::redo);
    disconnect(findAction,&QAction::triggered,editor,&CodeEditor::CreateFindDialog);
    disconnect(replaceAction,&QAction::triggered,editor,&CodeEditor::CreateReplaceDialog);
}

QString MainWindow::GetCurrentFilePath(){
    return FileSystem::getInstance().files[editTab->currentIndex()].currentFilePath;
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(FileSystem::getInstance().files.count() == 0)
        return;

    int ret = QMessageBox::question(this,"Hint","Do you need to save all files before you close?",
                                    QMessageBox::SaveAll,QMessageBox::No,QMessageBox::Cancel);
    if(ret == QMessageBox::SaveAll){
        for(int i=0;i<FileSystem::getInstance().files.count();i++){
            FileSystem::getInstance().files[i].content = editors[i]->toPlainText();
            FileSystem::getInstance().Save(i);
        }
    }
    else if(ret == QMessageBox::Cancel){
        event->ignore();
    }
    else{
        event->accept();
    }
}

void MainWindow::TranslateEditTab(){
    if(editTab->count()==0)
        return;

    lastEditor = currentEditor;
    currentEditor = editors[editTab->currentIndex()];

    DisconnectEditAction(lastEditor);
    ConnectEditAction(currentEditor);
}

void MainWindow::CloseTab(int index){
    int ret = QMessageBox::question(this,"Hint","Do you want save before close the file?",
                                    QMessageBox::Save,QMessageBox::No,QMessageBox::Cancel);
    if(ret == QMessageBox::Cancel)
        return;
    else if(ret == QMessageBox::Save){
        FileSystem::getInstance().files[index].content = editors[index]->toPlainText();
        FileSystem::getInstance().Save(index);
    }

    /*清除对应文件*/
    FileSystem::getInstance().files.removeAt(index);

    /*清除editor*/
    CodeEditor* editor = editors[index];
    DisconnectEditAction(editor);
    editors.removeAt(index);

    editTab->removeTab(index);
}


