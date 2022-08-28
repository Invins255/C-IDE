#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include "codeeditor.h"
#include <QVector>
#include <QObject>

struct file{
    QString content = "";
    QString currentFilePath = "";
    //是否为新建文件且已打开的标志
    bool fileIsNew = false;
    //是否已打开文件且不是新建的标志
    bool fileIsOpen = false;
};

class FileSystem:public QObject
{
    Q_OBJECT
public:
    static FileSystem& getInstance(){
        static FileSystem instance;
        return instance;
    }

    void NewFile();
    void Open();
    void Save(int index);
    void SaveAs(int index);
    QStringList GetAllFilePaths();

public:
    QVector<file> files;
private:
    FileSystem();
    FileSystem(FileSystem&) = delete;
    FileSystem& operator=(FileSystem&) = delete;

signals:
    void addFile(const QString& filePath);
};

#endif // FILESYSTEM_H
