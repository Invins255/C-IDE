#include "filesystem.h"
#include <QFileDialog>
#include <QMessageBox>


FileSystem::FileSystem()
{
    currentFilePath = "";
    fileIsNew = false;
    fileIsOpen = false;
}

void FileSystem::NewFile(){
    fileIsNew = true;
    fileIsOpen = true;
    //调用Save函数
    content = "";
    this->Save();
}

void FileSystem::Open(){
    fileName = QFileDialog::getOpenFileName(NULL,"打开文件","/","*.c");//函数返回路径+文件名

    if(fileName.isEmpty()){
        return;
    }

    FILE *p = fopen(fileName.toStdString().data(),"r");
    if(p == NULL){
        QMessageBox::information(NULL,"错误","打开文件失败");
    }
    else{
        while(!feof(p)){
            char buf[1024]={0};
            fgets(buf,sizeof(buf),p);
            content += buf;
        }
        currentFilePath = fileName;
        fileIsOpen = true;
        fclose(p);
    }
}

void FileSystem::Save(){
    if(fileIsNew){//若未保存过
        saveFileName = QFileDialog::getSaveFileName(NULL,"保存文件","/","*.c");
    }
    else{//若保存过
        saveFileName = currentFilePath;
    }
    if(saveFileName==NULL){
        return;
    }
    FILE *p=fopen(saveFileName.toStdString().data(),"w");
    if(p==NULL){
        QMessageBox::information(NULL,"错误","保存文件失败");
        return;
    }
    else{
        // text1->toPlainText().toStdString().data();//将用户在控件中输入的字符串转化为const char *
        fputs(content.toStdString().data(),p);
        fclose(p);
        currentFilePath = saveFileName;
        fileIsNew = false;
    }
}

void FileSystem::SaveAs(){
    saveFileName = QFileDialog::getSaveFileName(NULL,"另存为","/","*.c");
    if(saveFileName==NULL){
        return;
    }
    FILE *p=fopen(saveFileName.toStdString().data(),"w");
    if(p==NULL){
        QMessageBox::information(NULL,"错误","保存文件失败");
        return;
    }
    else{
        // text1->toPlainText().toStdString().data();//将用户在控件中输入的字符串转化为const char *
        fputs(content.toStdString().data(),p);
        fclose(p);
        currentFilePath = saveFileName;
        fileIsNew = false;
    }
}
