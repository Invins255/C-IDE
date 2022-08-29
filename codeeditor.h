#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QLineEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextDocument>
#include "highlighter.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void resizeEvent(QResizeEvent *event) override;

public slots:
    //查找槽函数
    void CreateFindDialog();
    void btnFind_slot();
    void btnFinishFind_slot();
    void btnFindNext_slot();
    void change_slot();
    //替换槽函数
    void CreateReplaceDialog();
    void btnReplace_slot();
    void btnFinish_slot();

    void deleteText();

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget *lineNumberArea;
    //用于查找
    QLineEdit *findLineEdit;
    QDialog *findDlg;
    //用于替换
    QDialog *replaceDlg;
    QLineEdit *formerLineEdit;
    QLineEdit *currentLineEdit;

    //代码高亮
    Highlighter* highlighter;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H

//该类参考自Qt演示示例code editor example
