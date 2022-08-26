#ifndef COMPILER_H
#define COMPILER_H

#include <QString>
#include <QProcess>
#include <QDebug>
#include <windows.h>

class Compiler
{
public:
    static Compiler& getInstance(){
        static Compiler instance;
        return instance;
    }

    void Compile(const QString& filePath);
    void Run(const QString& filePath);

    const QString& StandardOutput() const {return standardOutput;}
    const QString& StandardError() const {return standardError;}
private:
    Compiler();
    Compiler(Compiler&) = delete;
    Compiler& operator=(Compiler&) = delete;

    QString standardOutput;
    QString standardError;

    QString GetFileNameFromPath(QString path);
};

#endif // COMPILER_H
