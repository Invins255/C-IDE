#include "compiler.h"

Compiler::Compiler()
{

}

void Compiler::Compile(const QString& filePath){
    QString fileName = GetFileNameFromPath(filePath);

    QProcess process(nullptr);
    QStringList args;
    args.append("-o");
    args.append(fileName);//编译生成同名的exe文件
    args.append(filePath);
    process.start("gcc",args);
    process.waitForStarted();
    process.waitForFinished();
    standardOutput = QString::fromLocal8Bit(process.readAllStandardOutput());
    standardError = QString::fromLocal8Bit(process.readAllStandardError());
}

void Compiler::Run(const QString& filePath){  
    QProcess process(nullptr);
    process.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args)
    {
        args->flags |= CREATE_NEW_CONSOLE;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
        args->startupInfo->dwFillAttribute =  FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    });
    process.start(filePath);
    process.waitForStarted();
    process.waitForFinished();
    standardOutput = QString::fromLocal8Bit(process.readAllStandardOutput());
    standardError = QString::fromLocal8Bit(process.readAllStandardError());
}

QString Compiler::GetFileNameFromPath(QString path){
    QStringList list;
    if(path.contains('\\'))
        list = path.split("\\");
    else if(path.contains('/'))
        list = path.split('/');

    QString fileNameWithExt = list[list.count()-1];
    QString fileName = fileNameWithExt.split('.')[0];
    return fileName;
}
