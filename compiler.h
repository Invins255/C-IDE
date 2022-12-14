#ifndef COMPILER_H
#define COMPILER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QDebug>
#include <windows.h>

class Compiler:public QObject
{
    Q_OBJECT
public:
    static Compiler& getInstance(){
        static Compiler instance;
        return instance;
    }

    void Compile(const QString& filePath);      //单文件编译
    void Compile(const QStringList& filePaths);       //多文件编译
    void Run(const QString& filePath);

    const QString& StandardOutput() const {return standardOutput;}
    const QString& StandardError() const {return standardError;}

    static QString GetFileNameFromPath(QString path);
    static QString RemoveExtension(QString path);
private:
    Compiler();
    Compiler(Compiler&) = delete;
    Compiler& operator=(Compiler&) = delete;

    QString standardOutput;
    QString standardError;

    void CallCompiler(QStringList& args);

signals:
    void startCompiling();
    void startRunning();
    void compileFinished();
    void runFinished();
};

#endif // COMPILER_H
