#include "filesystem.h"

FileSystem::FileSystem()
{
    currentFilePath = "";
    currentEditor = nullptr;
}

void FileSystem::SetCurrentEditor(CodeEditor* pEditor){
    currentEditor = pEditor;
}

void FileSystem::NewFile(){

}

void FileSystem::Open(){

}

void FileSystem::Save(){

}

void FileSystem::SaveAs(){

}
