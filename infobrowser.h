#ifndef INFOBROWSER_H
#define INFOBROWSER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextDocument>
#include <QPlainTextEdit>

class InfoHighlighter:public QSyntaxHighlighter
{
    Q_OBJECT
public:
    InfoHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat errorFormat;
    QTextCharFormat warningFormat;
};


class InfoBrowser : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit InfoBrowser(QWidget *parent = nullptr);

private:
    InfoHighlighter* highlighter;
};


#endif // INFOBROWSER_H
