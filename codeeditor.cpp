#include <QtWidgets>

#include "codeeditor.h"

//![constructor]

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);

    QFont font;
    QFontMetrics metrics(font);
    setTabStopWidth(4 * metrics.width(' '));

    font.setPointSize(12);
    setFont(font);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

//![constructor]

//![extraAreaWidth]

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 5 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

//![slotUpdateRequest]

//![resizeEvent]

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//![resizeEvent]

//![cursorPositionChanged]

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
//![extraAreaPaintEvent_2]

void CodeEditor::CreateFindDialog(){
    findDlg = new QDialog(this);
    findDlg->setFixedSize(300,200);
    findDlg->setWindowTitle("Find");
    findLineEdit = new QLineEdit(findDlg);//创建QLineEdit
    QPushButton *btnFind= new QPushButton("Find", findDlg);//创建查找按钮
    QPushButton *btnFinishFind= new QPushButton("Finish", findDlg);//创建结束查找按钮
    QPushButton *btnFindNext= new QPushButton("Next", findDlg);//创建查找下一个按钮
    QVBoxLayout *layout= new QVBoxLayout(findDlg);//创建layaout
    layout->addWidget(findLineEdit);//将创建的QLineEdit、Button放置在layout上
    layout->addWidget(btnFind);
    layout->addWidget(btnFindNext);
    layout->addWidget(btnFinishFind);

    connect(btnFind,&QPushButton::clicked,this,this->btnFind_slot);
    connect(btnFinishFind,&QPushButton::clicked,this,this->btnFinishFind_slot);
    connect(btnFindNext,&QPushButton::clicked,this,this->btnFindNext_slot);

    findDlg->show();
}

void CodeEditor::btnFind_slot(){
    QString str = findLineEdit->text();
    QTextDocument *doc = this->document();
    int flag = 0;
    bool found = false;
    QTextCursor cursor(doc);
    QTextCharFormat format(cursor.charFormat());
    format.setForeground(Qt::black);
    format.setBackground(Qt::yellow);  //把找到的字背景设成黄色高亮显示
    while (!cursor.isNull() && !cursor.atEnd() && str!=""){
        //查找指定的文本，匹配整个单词
        cursor = doc->find(str, cursor, QTextDocument::FindWholeWords);
        if (!cursor.isNull())
        {
            if (!found)
                found = true;
            cursor.mergeCharFormat(format);
            flag++;
        }
    }
    if(flag==0 && str!="") {
        QMessageBox::warning(this, "Warning",QString("Fail to find:%1").arg(str));
    }
}

void CodeEditor::btnFinishFind_slot(){
    QString str = findLineEdit->text();
    QTextDocument *doc = this->document();
    bool found = false;
    QTextCursor cursor(doc);
    QTextCharFormat format(cursor.charFormat());
    format.setForeground(Qt::black);
    format.setBackground(Qt::white); //变回原来的白色背景
    while (!cursor.isNull() && !cursor.atEnd()&&str!="")
    {
        cursor = doc->find(str, cursor, QTextDocument::FindWholeWords);
        if (!cursor.isNull())
        {
            if (!found)
                found = true;
            cursor.mergeCharFormat(format);
        }
    }
    findLineEdit->clear();
    findDlg->close();
}

void CodeEditor::btnFindNext_slot(){
    QString str = findLineEdit->text();
    QTextDocument *doc = this->document();
    bool found = false;
    QTextCursor cursor(doc);
    QTextCharFormat format(cursor.charFormat());
    format.setForeground(Qt::black);
    format.setBackground(Qt::white);
    while (!cursor.isNull() && !cursor.atEnd()&&str!="")
    {
        cursor = doc->find(str, cursor, QTextDocument::FindWholeWords);
        if (!cursor.isNull())
        {
            if (!found) found = true;
            cursor.mergeCharFormat(format);
        }
    }
    findLineEdit->clear();
}

void CodeEditor::CreateReplaceDialog(){
    replaceDlg=new QDialog;
    replaceDlg->setWindowTitle("Replace");
    QLabel *lable1= new QLabel("Former:", replaceDlg);
    formerLineEdit =new QLineEdit(replaceDlg);
    QLabel *lable2= new QLabel("New:", replaceDlg);
    currentLineEdit =new QLineEdit(replaceDlg);
    QVBoxLayout *layout= new QVBoxLayout(replaceDlg);
    layout->addWidget(lable1);
    layout->addWidget(formerLineEdit);
    layout->addWidget(lable2);
    layout->addWidget(currentLineEdit);
    QPushButton *btnReplace= new QPushButton("Replace", replaceDlg);
    QPushButton *btnFinish= new QPushButton("Finish", replaceDlg);
    layout->addWidget(btnReplace);
    layout->addWidget(btnFinish);

    connect(btnReplace,&QPushButton::clicked,this,this->btnReplace_slot);
    connect(btnFinish,&QPushButton::clicked,this,this->btnFinish_slot);

    replaceDlg->show();
}

void CodeEditor::btnReplace_slot(){
    QString formerStr = formerLineEdit->text();
    QString currentStr = currentLineEdit->text();
    //QString str3;
    QTextDocument *doc = this->document();
    bool found = false;
    QTextCursor replace_cursor(doc);
    while (!replace_cursor.isNull() && !replace_cursor.atEnd()&&currentStr!=""&&formerStr!="")
    {
        //查找指定的文本，匹配整个单词
        replace_cursor = doc->find(formerStr, replace_cursor, QTextDocument::FindWholeWords);
        if (!replace_cursor.isNull())
        {
            if (!found)
                found = true;
            replace_cursor.removeSelectedText();
            replace_cursor.insertText(currentStr);
        }
    }
    formerLineEdit->clear();
    currentLineEdit->clear();
}

void CodeEditor::btnFinish_slot(){
    replaceDlg->close();
}

