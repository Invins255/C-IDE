#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include "codeeditor.h"

class FileSystem
{
public:
    static FileSystem& getInstance(){
        static FileSystem instance;
        return instance;
    }

    //TODO: finish these functions
    void NewFile();
    void Open();
    void Save();//保存
    void SaveAs();//另存为

public:
    QString content;
    QString currentFilePath;
private:
    bool fileIsNew;//是否为新建文件且已打开的标志
    bool fileIsOpen;//是否已打开文件且不是新建的标志
    QString fileName;//打开的文件名
    QString saveFileName;//保存的文件名

private:
    FileSystem();
    FileSystem(FileSystem&) = delete;
    FileSystem& operator=(FileSystem&) = delete;
};

#endif // FILESYSTEM_H
