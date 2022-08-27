#include "compiler.h"

Compiler::Compiler()
{

}

void Compiler::Compile(const QString& filePath){
    QStringList args;
    args.append(filePath);
    CallCompiler(args);
}

void Compiler::Compile(const QStringList& filePaths){
    QStringList args;
    for(int i=0;i<filePaths.count();i++)
        args.append(filePaths[i]);
    CallCompiler(args);
}

void Compiler::CallCompiler(QStringList& args){
    QString fileName = RemoveExtension(args[0]);

    args.append("-Wall");       //gcc指令参数:警告提示
    args.append("-pedantic");   //gcc指令参数:语法检查反馈错误
    args.append("-o");          //gcc指令参数:生成可执行文件
    args.append(fileName);      //编译生成同名的exe文件

    emit startCompiling();

    QProcess process(nullptr);
    process.start("gcc",args);
    process.waitForStarted();
    process.waitForFinished();
    standardOutput = QString::fromLocal8Bit(process.readAllStandardOutput());
    standardError = QString::fromLocal8Bit(process.readAllStandardError());

    emit compileFinished();
}

void Compiler::Run(const QString& filePath){  
    if(filePath.isEmpty()||!filePath.contains(".exe"))
        return;

    QProcess process(nullptr);
    process.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args)
    {
        args->flags |= CREATE_NEW_CONSOLE;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
        args->startupInfo->dwFillAttribute =  FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    });

    emit startRunning();

    process.start(filePath);
    process.waitForStarted();
    process.waitForFinished();
    standardOutput = QString::fromLocal8Bit(process.readAllStandardOutput());
    standardError = QString::fromLocal8Bit(process.readAllStandardError());

    emit runFinished();
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

QString Compiler::RemoveExtension(QString path){
    if(!path.contains("."))
        return path;
    return path.split('.')[0];
}
