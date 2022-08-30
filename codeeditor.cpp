#include <QtWidgets>

#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{   
    lineNumberArea = new LineNumberArea(this);

    setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);

    QFont font;
    font.setPointSize(10);
    font.setFamily("Hack");
    QFontMetrics metrics(font);
    setTabStopWidth(4 * metrics.width(' '));

    setFont(font);


    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    highlighter = new Highlighter(document());

    /*代码自动对齐*/
    connect(this,&CodeEditor::blockCountChanged,this,&CodeEditor::CodeAlign);
}


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


void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::CodeAlign(int newBlockCount){
    if(newBlockCount>lastBlockCount&&lastBlockCount!=0){
        QString lastLineStr = "";
        if(textCursor().blockNumber() > 0)
            lastLineStr = document()->findBlockByLineNumber(textCursor().blockNumber() - 1).text();

        auto tabsList = lastLineStr.split(QRegularExpression("[^\\t]+"));
        QString tabs=tabsList[0];
        insertPlainText(tabs);
    }

    lastBlockCount = newBlockCount;
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


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


void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

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

void CodeEditor::CreateFindDialog(){
    if(findDlg != nullptr){
        findDlg->show();
        return;
    }

    findDlg = new QDialog(this,Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    findDlg->setFixedSize(300,150);
    findDlg->setWindowTitle("Find");
    findLineEdit = new QLineEdit(findDlg);//创建QLineEdit
    QPushButton *btnFind= new QPushButton("Find", findDlg);//创建查找按钮
    QPushButton *btnFinishFind= new QPushButton("Finish", findDlg);//创建结束查找按钮

    QVBoxLayout *layout = new QVBoxLayout(findDlg);
    QHBoxLayout *layoutDown = new QHBoxLayout();
    layout->addWidget(findLineEdit);
    layoutDown->addWidget(btnFind);
    layoutDown->addWidget(btnFinishFind);
    layout->addLayout(layoutDown);

    connect(btnFind,&QPushButton::clicked,this,this->btnFind_slot);
    connect(btnFinishFind,&QPushButton::clicked,this,this->btnFinishFind_slot);
    connect(findLineEdit,&QLineEdit::textChanged,this,this->change_slot);

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

void CodeEditor::change_slot(){
    QTextDocument *doc = this->document();
    QTextCursor cursor(doc);
    QTextCharFormat format(cursor.charFormat());
    format.setForeground(Qt::black);
    format.setBackground(Qt::white);
    cursor.movePosition(QTextCursor::End,QTextCursor::KeepAnchor);//一个光标移到全文末尾，一个保持位置，相当于选中全文
    cursor.mergeCharFormat(format);//将全文背景变为白色
}

void CodeEditor::CreateReplaceDialog(){
    if(replaceDlg != nullptr){
        replaceDlg->show();
        return;
    }

    replaceDlg = new QDialog(this,Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    replaceDlg->setWindowTitle("Replace");
    replaceDlg->setFixedSize(300,150);

    QLabel *labelFormer = new QLabel("Former:", replaceDlg);
    formerLineEdit = new QLineEdit(replaceDlg);
    QLabel *labelNew = new QLabel("New:", replaceDlg);
    currentLineEdit =new QLineEdit(replaceDlg);

    QPushButton *btnReplace= new QPushButton("Replace", replaceDlg);
    QPushButton *btnShow = new QPushButton("Show",replaceDlg);
    QPushButton *btnFinish= new QPushButton("Finish", replaceDlg);

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->addWidget(labelFormer,0,0);
    gridLayout->addWidget(formerLineEdit,0,1);
    gridLayout->addWidget(labelNew,1,0);
    gridLayout->addWidget(currentLineEdit,1,1);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(btnShow);
    buttonsLayout->addWidget(btnReplace);
    buttonsLayout->addWidget(btnFinish);

    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->addLayout(gridLayout);
    vlayout->addLayout(buttonsLayout);
    replaceDlg->setLayout(vlayout);

    connect(btnReplace,&QPushButton::clicked,this,this->btnReplace_slot);
    connect(btnFinish,&QPushButton::clicked,this,this->btnFinish_slot);
    connect(btnShow,&QPushButton::clicked,this,this->btnShow_slot);
    connect(formerLineEdit,&QLineEdit::textChanged,this,this->change_slot);
    connect(btnFinish,&QPushButton::clicked,this,this->change_slot);

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

void CodeEditor::btnShow_slot(){
    QString str = formerLineEdit->text();
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
}

void CodeEditor::deleteText(){
    textCursor().removeSelectedText();
}
