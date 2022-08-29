#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent):QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    /*Format settings*/
    //keywordFormat
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::DemiBold);
    QStringList keywordPatterns;
    keywordPatterns << "char" << "void" << "int" << "float" << "double"
                    << "short" << "long" << "signed" << "unsigned" << "struct"
                    << "union" << "enum" << "typedef" << "sizeof" << "auto"
                    << "static" << "register" << "extern" << "const" << "volatile"
                    << "return" << "continue" << "break" << "goto" << "if"
                    << "else" << "switch" << "case" << "default" << "for"
                    << "do" << "while";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    //includeFormat
    includeFormat.setForeground(QBrush(QColor(132, 112, 255)));
    includeFormat.setFontWeight(QFont::DemiBold);
    rule.pattern = QRegularExpression("#include");
    rule.format = includeFormat;
    highlightingRules.append(rule);

    //quotationFormat
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //CommentFormat
    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");

    //functionFormat
    functionFormat.setForeground(Qt::darkYellow);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);
}


void Highlighter::highlightBlock(const QString &text)
{
    //处理普通模式
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    // 处理注释
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
