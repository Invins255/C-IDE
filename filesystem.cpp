#include "filesystem.h"
#include <QFileDialog>
#include <QMessageBox>


FileSystem::FileSystem()
{

}

void FileSystem::NewFile(){
    file f;
    f.fileIsNew=true;
    f.fileIsOpen=true;
    files.append(f);
    //调用Save函数
    this->Save(files.count()-1);

    emit addFile(files.last().currentFilePath);
}

void FileSystem::Open(){
    QString fileName;
    fileName = QFileDialog::getOpenFileName(NULL,"打开文件","/","*.c *.h");//函数返回路径+文件名

    if(fileName.isEmpty()){
        return;
    }

    file f;
    files.append(f);

    FILE *p = fopen(fileName.toStdString().data(),"r");
    if(p == NULL){
        QMessageBox::information(NULL,"错误","打开文件失败");
    }
    else{
        while(!feof(p)){
            char buf[1024]={0};
            fgets(buf,sizeof(buf),p);
            files.last().content += buf;
        }
        files.last().currentFilePath = fileName;
        files.last().fileIsOpen = true;
        fclose(p);
    }

    emit addFile(files.last().currentFilePath);
}

void FileSystem::Save(int index){
    QString saveFileName;
    if(files[index].fileIsNew){//若未保存过
        saveFileName = QFileDialog::getSaveFileName(NULL,"保存文件","/","*.c *.h");
    }
    else{//若保存过
        saveFileName = files[index].currentFilePath;
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
        fputs(files[index].content.toStdString().data(),p);
        fclose(p);
        files[index].currentFilePath = saveFileName;
        files[index].fileIsNew = false;
    }
}

void FileSystem::SaveAs(int index){
    QString saveFileName;
    saveFileName = QFileDialog::getSaveFileName(NULL,"另存为","/","*.c *.h");
    if(saveFileName==NULL){
        return;
    }
    FILE *p=fopen(saveFileName.toStdString().data(),"w");
    if(p==NULL){
        QMessageBox::information(NULL,"错误","保存文件失败");
        return;
    }
    else{
        fputs(files[index].content.toStdString().data(),p);
        fclose(p);
        files[index].currentFilePath = saveFileName;
        files[index].fileIsNew = false;
    }
}

QStringList FileSystem::GetAllFilePaths(){
    QStringList list;
    for(int i=0;i<files.count();i++){
        list.append(files[i].currentFilePath);
    }
    return list;
}
