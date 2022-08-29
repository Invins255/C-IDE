#include "infobrowser.h"

InfoBrowser::InfoBrowser(QWidget *parent) : QPlainTextEdit(parent)
{
    QFont font;
    font.setPointSize(11);
    font.setWeight(QFont::Medium);
    setFont(font);

    highlighter = new InfoHighlighter(document());
}


InfoHighlighter::InfoHighlighter(QTextDocument *parent):QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    errorFormat.setForeground(Qt::red);
    errorFormat.setFontPointSize(13);
    rule.pattern = QRegularExpression("error\\:");
    rule.format = errorFormat;
    highlightingRules.append(rule);

    warningFormat.setForeground(QBrush(QColor(255,215,0)));
    warningFormat.setFontPointSize(13);
    rule.pattern = QRegularExpression("warning\\:");
    rule.format = warningFormat;
    highlightingRules.append(rule);
}

void InfoHighlighter::highlightBlock(const QString &text){
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
}
