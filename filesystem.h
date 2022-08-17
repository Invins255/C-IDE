#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include "codeeditor.h"

class FileSystem
{
public:
    static FileSystem& getInstance(){
        static FileSystem instance;
        return instance;
    }

    void SetCurrentEditor(CodeEditor* pEditor);

    //TODO: finish these functions
    void NewFile();
    void Open();
    void Save();
    void SaveAs();

private:
    FileSystem();
    FileSystem(FileSystem&) = delete;
    FileSystem& operator=(FileSystem&) = delete;

    std::string currentFilePath;
    CodeEditor* currentEditor;
};

#endif // FILESYSTEM_H
