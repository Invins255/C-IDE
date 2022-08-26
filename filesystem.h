#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QString>
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
    void Save();
    void SaveAs();

    const QString& GetCurrentFilePath() const {return currentFilePath;}
private:
    FileSystem();
    FileSystem(FileSystem&) = delete;
    FileSystem& operator=(FileSystem&) = delete;

    QString currentFilePath;
};

#endif // FILESYSTEM_H
